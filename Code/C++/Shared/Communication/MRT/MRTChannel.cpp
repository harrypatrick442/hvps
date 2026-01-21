#include "MRTChannel.hpp"
#include "System/SafeAbort.hpp"
#include "Logging/Log.hpp"
#include "IO/IOInteruptHelper.hpp"
#include "Timing/CPUClockFrequencyHelper.hpp"
#include "Timing/Delay.hpp"
#include "driver/gptimer.h"
#include <cstdio>
#include <cstring>

MRTChannel::MRTChannel(
    int txPin,
    int rxPin,
    int periodUs,
	bool invertTx,
	bool invertRx,
	size_t receiveQueueSize
) :
    _txPin(txPin),
    _rxPin(rxPin),
	_txGPIONum((gpio_num_t)_txPin),
	_rxGPIONum((gpio_num_t)_rxPin),
	_invertTx(invertTx),
	_invertRx(invertRx),
	_periodUs(periodUs),
	_syncPulseMinCCycles(nSubPulsesToCCycles(1)),
	_syncPulseSubPulses(2),
	_syncPulseMaxCCycles(nSubPulsesToCCycles(3)),
    _zeroPulseSubPulses(4),
	_zeroPulseMaxCCycles(nSubPulsesToCCycles(5)),
    _onePulseSubPulses(6),
	_onePulseMaxCCycles(nSubPulsesToCCycles(N_SUB_PULSES_PER_PULSE)),
	_currentByte(0),
	_nextNBit(0),
	_writeTimerPeriodUs(periodUs/N_SUB_PULSES_PER_PULSE),
	_writeTimer(_writeTimerPeriodUs, ESP_INTR_FLAG_LEVEL3, true),
	_rxSymbolQueue(nullptr),
	_symbolsBeingWritten(nullptr),
	_symbolsBeingWrittenLength(0),
	_txISRSymbolIndex(0),
	_txISRSubPulsesIntoCurrentSymbol(N_SUB_PULSES_PER_PULSE),
	_txISRCurrentSymbol(MRTSymbol::Sync),
	_nextWriteBufferForISRFreeLatch(false),
	_rxHandlerInitialized(false),
	_rxHandlerLastWasHigh(false),
	_rxHandlerHighCCycles(0),
	_successfullyConfigured(false),
	_createdRxEdgeInterupt(false)
{
	if(receiveQueueSize<=0){
		receiveQueueSize = 256;
	}
    std::snprintf(_description, sizeof(_description),
        "MRTChannel");
	if (periodUs < N_SUB_PULSES_PER_PULSE) {
		SAFE_ABORT("periodUs too small for pulse scheme");
	}
	_rxSymbolQueue = xQueueCreate(
        receiveQueueSize,
        sizeof(MRTSymbol)
    );
	if (!_rxSymbolQueue) {
		SAFE_ABORT("Failed to create RX symbol queue");
	}
}

MRTChannel::~MRTChannel() {
    std::lock_guard<std::mutex> lock(_txMutex);//Wait for exclusive thread access to the latch
    _nextWriteBufferForISRFreeLatch.wait();//Wait for the current write to finish before destroying timer
	freeWriteTimerIfCreated();
	IOInteruptHelper::removeHandlerAndDisableEdgeInterupt(_rxGPIONum);
	if (_rxSymbolQueue != nullptr) {
		vQueueDelete(_rxSymbolQueue);
		_rxSymbolQueue = nullptr; // prevent accidental reuse
	}
	if(_symbolsBeingWritten!=nullptr){
		delete[] _symbolsBeingWritten;
		_symbolsBeingWritten = nullptr;
	}
}

void MRTChannel::addSyncPulse(MRTSymbol* items, size_t& nextSymbolIndex) {
    items[nextSymbolIndex++] = MRTSymbol::Sync;
}

bool MRTChannel::configure() {
    std::lock_guard<std::mutex> lock(_configureMutex);
	if(_successfullyConfigured.load(std::memory_order_relaxed)){
		SAFE_ABORT("Already configured");
		return false;
	}
    if(!configureTx()){
		return false;
	}
	if(!configureRx()){
		return false;
	}
	if(!configureTxTimer()){
		return false;
	}
	_successfullyConfigured.store(true, std::memory_order_relaxed);
	return true;
}

bool MRTChannel::configureRx() {
	if(_createdRxEdgeInterupt.load(std::memory_order_relaxed)){
		IOInteruptHelper::removeHandlerAndDisableEdgeInterupt(_rxGPIONum);
	}
	esp_err_t err = IOInteruptHelper::setupPinEdgeInterupt(
		_rxPin,
		rxEdgeISRTrampoline,
		(void*)this,
		true,	//risingEdg
		true,	//fallingEdge
		false,	//pullUpEnabled
		false	//pullDownEnabled
	);
	if(err!=ESP_OK){
		SAFE_ABORT("Failed to configure rx pin edge interupt with error: %s", esp_err_to_name(err));
		return false;
	}
	_createdRxEdgeInterupt.store(true, std::memory_order_relaxed);
	return true;
}

bool MRTChannel::configureTx() {
    gpio_config_t io_conf_tx = {};
    io_conf_tx.pin_bit_mask = (1ULL << _txPin);
    io_conf_tx.mode = GPIO_MODE_OUTPUT;
    esp_err_t err = gpio_config(&io_conf_tx);
	
	if(err!=ESP_OK){
		SAFE_ABORT("Failed to configure tx pin with error: %s", esp_err_to_name(err));
		return false;
	}
    gpio_set_level(_txGPIONum, _invertTx ? 1 : 0);
	return true;
}

bool MRTChannel::configureTxTimer(){
	freeWriteTimerIfCreated();
	
	return _writeTimer.configure(&MRTChannel::txTimerISRTrampoline, this)==ESP_OK;
	/*
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_APB,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000ULL, // 1 MHz = 1us per tick
    };
	esp_err_t err = gptimer_new_timer(&config, &_writeTimer);
	char* methodNameFailedOn = nullptr;
	while(true){
		if(err!= ESP_OK) {
			methodNameFailedOn = "gptimer_new_timer";
			return false;
		}
		gptimer_event_callbacks_t cbs = {
			.on_alarm = &MRTChannel::txTimerISRTrampoline, // must match signature
		};
		err = gptimer_register_event_callbacks(_writeTimer, &cbs, this);
		if(err!= ESP_OK) {
			methodNameFailedOn = "gptimer_register_event_callbacks";
			break;
		}
		
		err =gptimer_enable(_writeTimer);
		if(err!= ESP_OK) {
			methodNameFailedOn = "gptimer_enable";
			break;
		}
		gptimer_alarm_config_t alarm_cfg = {};
		alarm_cfg.alarm_count = _writeTimerPeriodUs;
		alarm_cfg.reload_count = 0;
		alarm_cfg.flags.auto_reload_on_alarm = true;
		err = gptimer_set_alarm_action(_writeTimer, &alarm_cfg);
		if(err!= ESP_OK) {
			methodNameFailedOn = "gptimer_set_alarm_action";
			break;
		}
		return true;
	}
	SAFE_ABORT("Failed to create tx timer on %s with error: %s", methodNameFailedOn, esp_err_to_name(err));
	return false;*/
}

void MRTChannel::encodeByte(uint8_t b, MRTSymbol* items, size_t& nextSymbolIndex) {
	addSyncPulse(items, nextSymbolIndex);
    for (int bit = 7; bit >= 0; bit--) {
        bool one = (b >> bit) & 0x01;	
		items[nextSymbolIndex++] = one?(MRTSymbol::One):(MRTSymbol::Zero);
    }
}

void MRTChannel::flushTx() {
    std::lock_guard<std::mutex> lock(_txMutex);
    _nextWriteBufferForISRFreeLatch.wait();
}

esp_err_t MRTChannel::freeWriteTimerIfCreated() {
/*
    if (_writeTimer == nullptr)
        return ESP_OK;
    esp_err_t err = gptimer_stop(_writeTimer);
    if (err != ESP_OK) {
        LOG_WARN("Failed to stop gptimer: %s", esp_err_to_name(err));
    }

    err = gptimer_disable(_writeTimer);
    if (err != ESP_OK) {
        LOG_WARN("Failed to disable gptimer: %s", esp_err_to_name(err));
    }

    err = gptimer_del_timer(_writeTimer);
    if (err != ESP_OK) {
        LOG_WARN("Failed to delete gptimer: %s", esp_err_to_name(err));
    }
    _writeTimer = nullptr;
*/
	
    return _writeTimer.release();
}
const char* MRTChannel::getDescription() const {
    return _description;
}

void MRTChannel::handleMalformedByte(uint8_t nextNBit){
	return;
	//We dont really need this. Any noise will trigger it. Such as anything at startup*/
	static bool firstMalformedWarning = true;
	if(firstMalformedWarning){
		LOG_WARN("Received malformed byte with a length of %d", nextNBit);
		firstMalformedWarning = false;
	}
}

void IRAM_ATTR MRTChannel::handleTxTickFromISR(){
	if(_txISRSubPulsesIntoCurrentSymbol>=N_SUB_PULSES_PER_PULSE){
		if(_txISRSymbolIndex>=_symbolsBeingWrittenLength){
			_writeTimer.stop();
			_nextWriteBufferForISRFreeLatch.unlatchFromISR();
			return;
		}
		_txISRSubPulsesIntoCurrentSymbol = 0;
		_txISRCurrentSymbol  = _symbolsBeingWritten[_txISRSymbolIndex++];
	}
	setIOBasedOnSymbol(_txISRSubPulsesIntoCurrentSymbol++, _txISRCurrentSymbol);
}

void IRAM_ATTR MRTChannel::handleRxEdgeFromISR(){
	int level = gpio_get_level(_rxGPIONum);
	uint32_t nowCCycles = esp_cpu_get_cycle_count();
	if((level==1)^_invertRx){
		//High
		if(_rxHandlerLastWasHigh)return;
		if(!_rxHandlerInitialized){
			_rxHandlerInitialized = true;
		}
		_rxHandlerHighCCycles = nowCCycles;
		_rxHandlerLastWasHigh = true;
		return;
	}
	//Low
	if(!_rxHandlerInitialized){
		_rxHandlerLastWasHigh = false;
		_rxHandlerInitialized = true;
		return;
	}
	if(!_rxHandlerLastWasHigh)return;
	_rxHandlerLastWasHigh = false;
	uint32_t dCycles = nowCCycles - _rxHandlerHighCCycles;
	 MRTSymbol symbol;
    if (dCycles < _syncPulseMinCCycles) {
        return; // reject noise
    }
    else if (dCycles <= _syncPulseMaxCCycles) {
        symbol = MRTSymbol::Sync;
    }
    else if (dCycles <= _zeroPulseMaxCCycles) {
        symbol = MRTSymbol::Zero;
    }
    else if(dCycles <= _onePulseMaxCCycles) {
        symbol = MRTSymbol::One;
    }
	else{
		return;//Pulse too long something went wrong here so reject.
	}
	BaseType_t higherPriorityTaskWoken = pdFALSE;
	if (xQueueSendFromISR(_rxSymbolQueue, &symbol, &higherPriorityTaskWoken) == pdTRUE) {
        if (higherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
}

uint32_t MRTChannel::nSubPulsesToCCycles(int nSubPulses)
{
    uint64_t result =
        (static_cast<uint64_t>(_periodUs) *
         static_cast<uint64_t>(nSubPulses) *
         static_cast<uint64_t>(CPUClockFrequencyHelper::getClockFrequencyMHZExact())) / N_SUB_PULSES_PER_PULSE;

    if(result > UINT32_MAX)
        return UINT32_MAX;

    return static_cast<uint32_t>(result);
}
void MRTChannel::printTimingConfig() const
{
    LOG_INFO("=== MRT Timing Configuration ===");

    LOG_INFO("periodUs                = %u us", _periodUs);
    LOG_INFO("sub-pulses per pulse    = %d", N_SUB_PULSES_PER_PULSE);
    LOG_INFO("writeTimerPeriodUs      = %llu us", _writeTimerPeriodUs);

    LOG_INFO("--- Symbol sub-pulses ---");
    LOG_INFO("SYNC sub-pulses         = %u", _syncPulseSubPulses);
    LOG_INFO("ZERO sub-pulses         = %u", _zeroPulseSubPulses);
    LOG_INFO("ONE  sub-pulses         = %u", _onePulseSubPulses);

    LOG_INFO("--- Pulse width (CPU cycles) ---");
    LOG_INFO("SYNC min cycles         = %u", _syncPulseMinCCycles);
    LOG_INFO("SYNC max cycles         = %u", _syncPulseMaxCCycles);
    LOG_INFO("ZERO max cycles         = %u", _zeroPulseMaxCCycles);
    LOG_INFO("ONE  max cycles         = %u", _onePulseMaxCCycles);

    LOG_INFO("--- Derived rates ---");
    uint32_t pulsesPerByte = 9 + 1; // 8 bits + sync + trailing sync
    uint32_t usPerByte = pulsesPerByte * _periodUs;
    uint32_t bytesPerSecond = 1'000'000 / usPerByte;

    LOG_INFO("us per byte             = %u us", usPerByte);
    LOG_INFO("approx bytes/sec        = %u B/s", bytesPerSecond);

    LOG_INFO("================================");
}
size_t MRTChannel::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
	if(!_successfullyConfigured.load(std::memory_order_relaxed)){
		SAFE_ABORT("Did not initialize properly so cannot read or write bytes.");
		return 0;
	}
	size_t nextDestinationIndex = 0;
	MRTSymbol symbol;
	while(nextDestinationIndex<maxLength){
		if (xQueueReceive(_rxSymbolQueue, &symbol, pdMS_TO_TICKS(timeoutMs)) != pdTRUE) {
			break;
		}
		if(symbol==MRTSymbol::Sync){
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
			uint8_t bit = (symbol==MRTSymbol::One)?1:0;
			_currentByte = (_currentByte << 1) | bit;
		}
		_nextNBit++;
	}
    return nextDestinationIndex;
}

void IRAM_ATTR MRTChannel::rxEdgeISRTrampoline(void* arg) {
    static_cast<MRTChannel*>(arg)->handleRxEdgeFromISR();
}

void MRTChannel::scheduleWriteBuffer(MRTSymbol* symbols, size_t symbolsLength){
    std::lock_guard<std::mutex> lock(_txMutex);
    _nextWriteBufferForISRFreeLatch.wait();
    // Wait until ISR has freed the slot.

	if(_symbolsBeingWritten!=nullptr){
		delete[] _symbolsBeingWritten;
	}
    _symbolsBeingWritten = symbols;
	_symbolsBeingWrittenLength = symbolsLength;
	_txISRSubPulsesIntoCurrentSymbol = N_SUB_PULSES_PER_PULSE;
	_txISRSymbolIndex = 0;
    _nextWriteBufferForISRFreeLatch.latch();
	esp_err_t err = _writeTimer.start();
	if(err != ESP_OK){
		// extremely rare but worth catching
		delete[] _symbolsBeingWritten;
		_symbolsBeingWritten = nullptr;
		_symbolsBeingWrittenLength = 0;
		_nextWriteBufferForISRFreeLatch.unlatch(); 
		SAFE_ABORT("Failed to start TX timer");
		return;
	}
    _nextWriteBufferForISRFreeLatch.wait();
}

void IRAM_ATTR MRTChannel::setIOBasedOnSymbol(int8_t subPulsesIntoCurrentSymbol, MRTSymbol symbol){
	bool on;
	switch(symbol){
		case MRTSymbol::Zero:
			on = subPulsesIntoCurrentSymbol<_zeroPulseSubPulses;
			break;
		case MRTSymbol::One:
			on = subPulsesIntoCurrentSymbol<_onePulseSubPulses;
			break;
		case MRTSymbol::Sync:
		default:
			on = subPulsesIntoCurrentSymbol<_syncPulseSubPulses;
			break;
	}
	gpio_set_level(_txGPIONum, (on^_invertTx)?1:0);
}

bool IRAM_ATTR MRTChannel::txTimerISRTrampoline(void *arg)
{
    static_cast<MRTChannel*>(arg)->handleTxTickFromISR();
    return false;  // no context switch needed
}

size_t MRTChannel::writeBytes(const char* src, size_t len) {
	if(!_successfullyConfigured.load(std::memory_order_relaxed)){
		SAFE_ABORT("Did not initialize properly so cannot read or write bytes.");
		return 0;
	}
    MRTSymbol* symbols = new MRTSymbol[(len*9)+1];
	size_t nextSymbolIndex = 0;
    for (size_t i = 0; i < len; i++) {
        encodeByte(static_cast<uint8_t>(src[i]), symbols, nextSymbolIndex);
    }
	addSyncPulse(symbols, nextSymbolIndex);
	scheduleWriteBuffer(symbols, nextSymbolIndex);
    return len;
}