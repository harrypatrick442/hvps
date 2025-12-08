#include "HPulse.hpp"
#include "System/Aborter.hpp"
#include "Logging/Log.hpp"
#include <cstdio>
#include <cstring>
HPulse::HPulse(
    int txPin,
    int rxPin,
    int periodUs,
	bool invertTx,
	bool invertRx
) :
    _txPin(txPin),
    _rxPin(rxPin),
	_invertTx(invertTx),
	_invertRx(invertRx),
	_periodUs(periodUs),
	_syncPulseTicks(1),
	_syncPulseLowTicks(6 - _syncPulseTicks),
	_syncPulseMaxUs((periodUs*2)/6),
    _zeroPulseTicks(3),
    _zeroPulseLowTicks(6 - _zeroPulseTicks),
	_zerotPulseMaxUs((periodUs*4)/6),
    _onePulseTicks(5),
    _onePulseLowTicks(6 - _onePulseTicks),
	_currentByte(0),
	_nextNBit(0),
	_txChannelCreatedAndEnabled(false),
	_rxChannelCreatedAndEnabled(false),
	_writeTimer(nullptr),
	_rxSymbolQueue(nullptr)
{
    std::snprintf(_description, sizeof(_description),
        "HPulse");
	if (periodUs < 6) {
		Aborter::safeAbort(TAG, "periodUs too small for pulse scheme");
		return;
	}
	_rxSymbolQueue = xQueueCreate(
        RECEIVE_QUEUE_SIZE_SYMBOLS,
        sizeof(HSymbol)
    );
	if (!_rxSymbolQueue) {
		Aborter::safeAbort(TAG, "Failed to create RX symbol queue");
		return;
	}
	memset(_rxBuffer, 0, sizeof(_rxBuffer));
	
    esp_timer_create_args_t args = {
        .callback =  HPulse::onTxTimerISR,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "HPulse::_writeTimer"
    };
    esp_error_t err = esp_timer_create(&args, &_writeTimer);
	if(err!= ESP_OK) {
		Aborter::safeAbort(TAG, "Failed to create tx timer");
		return;
	}
}

HPulse::~HPulse() {
	YOU DONT YET CLEAN UP THE QUEUE AND TIMER AND STUFF PROPERLY HERE!
    if (_txChannelCreatedAndEnabled.load()) {
		rmt_disable(_txChannelHandle);
        rmt_del_channel(_txChannelHandle);
    }
    if (_rxChannelCreatedAndEnabled.load()) {
		rmt_disable(_rxChannelHandle);
        rmt_del_channel(_rxChannelHandle);
    }
}

bool HPulse::configure() {
    if(!configureTx()){
		return false;
	}
	if(!configureRx()){
		return false;
	}
	return true;
}
bool HPulse::configureRx() {
    gpio_config_t io_conf_rx = {};
    io_conf_rx.pin_bit_mask = (1ULL << _rxPin);
    io_conf_rx.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf_rx);
}
bool HPulse::configureTx() {
    gpio_config_t io_conf_tx = {};
    io_conf_tx.pin_bit_mask = (1ULL << _txPin);
    io_conf_tx.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf_tx);
    gpio_set_level((gpio_pin_num)_txNum, _invertTx ? 1 : 0);	
}

size_t HPulse::writeBytes(const char* src, size_t len) {
    HSymbol* symbols = new HSymbol[len];
	size_t nextSymbolIndex = 0;
    for (size_t i = 0; i < len; i++) {
        encodeByte(static_cast<uint8_t>(src[i]), symbols, nextSymbolIndex);
    }
	addSyncPulse(symbols, nextWriteBufferIndex);
	scheduleWriteBuffer(HBuffer(symbols, len));
    return len;
}

void HPulse::encodeByte(uint8_t b, HPulseSymbol* items, size_t& nextSymbolIndex) {
	addSyncPulse(items, nextSymbolIndex);
    for (int bit = 7; bit >= 0; bit--) {
        bool one = (b >> bit) & 0x01;	
		items[nextSymbolIndex++] = one?(HSymbol::One):(HSymbol::Zero);
    }
}

void HPulse::addSyncPulse(rmt_symbol_word_t* items, size_t& nextSymbolIndex) {
    items[nextSymbolIndex++] = HSymbol::Sync;
}
void HPulse::scheduleWriteBuffer(HSymbol* symbols, size_t symbolsLength){
    std::lock_guard<std::mutex> lock(_txMutex);
    // Wait until ISR has freed the slot.
    _writeBufferForISRFreeLatch.wait();

	if(_symbolsBeingWritten!=nullptr){
		delete[] _symbolsBeingWritten;
	}
    _symbolsBeingWritten = symbols;
	_symbolsBeingWrittenLength = symbolsLength;
	esp_timer_start_periodic(_writeTimer, _writeTimerPeriodUs);
    // Immediately relatch, so no other writers proceed until ISR clears it.
    _nextWriteBufferForISRFreeLatch.latch();
}

void HPulse::flushTx() {
	
}
void IRAM_ATTR HPulse::onTxTimerISR(void* arg)
{
    HPulse* self = static_cast<HPulse*>(arg);
    self->handleTxTickFromISR();
}
void IRAM_ATTR HPulse::handleTxTickFromISR(){
	static size_t symbolIndex = 0;
	static int8_t ticksIntoCurrentSymbol = 6;
	static Symbol currentSymbol;
	if(ticksIntoCurrentSymbol>=5){
		if(symbolIndex>=_symbolsBeingWrittenLength){			
			symbolIndex = 0;
			esp_timer_stop(_writeTimer);
			_nextWriteBufferForISRFreeLatch.unlatchFromISR();
			return;
		}
		ticksIntoCurrentSymbol = 0;
		currentSymbol  = _symbolsBeingWritten[symbolIndex++];
	}
	setIOBasedOnSymbol(ticksIntoCurrentSymbol++, currentSymbol);
}
void IRAM_ATTR setIOBasedOnSymbol(int8_t ticksIntoCurrentSymbol, HSymbol symbol){
	bool on;
	switch(symbol){
		case HSymbol::Zero:
			on = ticksIntoCurrentSymbol<=_zeroPulseTicks;
			break;
		case HSymbol::One:
			on = ticksIntoCurrentSymbol<=_onePulseTicks;
			break;
		case HSymbol::Sync:
		default:
			on = ticksIntoCurrentSymbol<=_syncPulseTicks;
			break;
	}
	gpio_set_level((gpio_pin_num)_txPin, (on^_invertTx)?1:0;
}
size_t HPulse::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
    if (!_rxSymbolQueue) return 0;
	size_t nextDestinationIndex = 0;
	HSymbol symbol;
	while(nextDestinationIndex<maxLength){
		if (xQueueReceive(_rxSymbolQueue, &symbol, pdMS_TO_TICKS(timeoutMs)) != pdTRUE) {
			break;
		}
		if(symbol==HSymbol::Sync){
			if(_nextNBit>0){	
				if(_nextNBit!=8){
					handleMalformedByte(_nextNBit);
				}
				destination[nextDestinationIndex++]=_currentByte;
				if(nextDestinationIndex>=maxLength){
					break;
				}
				_nextNBit = 0;
			}
			_currentByte = 0;
			continue;
		}
		if(_nextNBit<8){
			uint8_t bit = (symbol==HSymbol::One)?1:0;
			_currentByte = (_currentByte << 1) | bit;
		}
		_nextNBit++;
	}
    return nextDestinationIndex;
}
void HPulse::handleMalformedByte(uint8_t _nextNBit){
	//We dont really need this. Any noise will trigger it. Such as anything at startup*/
	static bool firstMalformedWarning = true;
	if(firstMalformedWarning){
		Log::Warn(TAG, "Received malformed byte with a length of %d", _nextNBit);
		firstMalformedWarning = false;
	}
}
THE TIME BETWEEN CYCLES.
uint32_t now  = xthal_get_ccount();
uint32_t last = _lastCycleStamp;
uint32_t dt_cycles = now - last;


const char* HPulse::getDescription() const {
    return _description;
}
size_t HPulse::getMinRequiredReceiveBufferSize() const{
	return MIN_REQUIRED_RECEIVE_BUFFER_SIZE;
}
bool HPulse::onReceiveStatic(
    rmt_channel_handle_t channel,
    const rmt_rx_done_event_data_t* edata,
    void* user_ctx
) {
    return static_cast<HPulse*>(user_ctx)->onReceive(edata);
}
bool HPulse::onReceive(const rmt_rx_done_event_data_t* edata) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // Iterate through all received symbols and copy each one into the queue safely from ISR
    for (size_t i = 0; i < edata->num_symbols; i++) {
        rmt_symbol_word_t symbol = edata->received_symbols[i];

        // Push the *copy* of the symbol struct into the queue
        if (xQueueSendFromISR(_rxSymbolQueue, &symbol, &higherPriorityTaskWoken) != pdPASS) {
			//handleDroppedSymbols();
        }
    }

	/*
	// CRITICAL: Re-arm the receiver for the next shot immediately after copying data
	esp_err_t err = rmt_receive(_rxChannelHandle, &_rxBuffer, READ_BUFFER_SIZE_SYMBOLS, &_rxReceiveConfig);
	if (err != ESP_OK) {
		// Handle failure to restart RMT reception
		handleFailedScheduleNextReceive(err);
	}
	*/
    // Return whether a higher-priority task should be context-switched to
    return higherPriorityTaskWoken == pdTRUE;
}
