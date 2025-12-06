#include "HardwareRMT.hpp"
#include "System/Aborter.hpp"
#include "Logging/Log.hpp"
#include <cstdio>
#include <cstring>
HardwareRMT::HardwareRMT(
    int txChannel,
    int rxChannel,
    int txPin,
    int rxPin,
    int periodUs,
	bool invertTx,
	bool invertRx
) :
    _txChannel(txChannel),
    _rxChannel(rxChannel),
    _txPin(txPin),
    _rxPin(rxPin),
	_invertTx(invertTx),
	_invertRx(invertRx),
	_periodUs(periodUs),
	_syncPulseUs((periodUs*1)/6),
	_syncPulseLowUs(periodUs - _syncPulseUs),
	_syncPulseMaxUs((periodUs*2)/6),
    _shortPulseUs((periodUs*3)/6),
    _shortPulseLowUs(periodUs - _shortPulseUs),
	_shortPulseMaxUs((periodUs*4)/6),
    _longPulseUs((periodUs*5)/6),
    _longPulseLowUs(periodUs - _longPulseUs),
    _rb(nullptr),
	_currentByte(0),
	_nextNBit(0)
{
	if (periodUs < 6) {
		Aborter::safeAbort(TAG, "periodUs too small for pulse scheme");
	}
    std::snprintf(_description, sizeof(_description),
        "RMT(tx:%d rx:%d)", txChannel, rxChannel);
}

HardwareRMT::~HardwareRMT() {
    if (_txChannel >= 0) {
        rmt_driver_uninstall((rmt_channel_t)_txChannel);
    }
    if (_rxChannel >= 0) {
        rmt_driver_uninstall((rmt_channel_t)_rxChannel);
    }
}

bool HardwareRMT::configure() {
    // --------------------
    //  CONFIGURE TX
    // --------------------
    if (_txChannel >= 0) {
        rmt_config_t txconf = {};
        txconf.rmt_mode = RMT_MODE_TX;
        txconf.channel = (rmt_channel_t)_txChannel;
        txconf.gpio_num = (gpio_num_t)_txPin;
        txconf.mem_block_num = 1;
        txconf.clk_div = 80;   // 1 tick = 1µs (80MHz / 80)

        if (rmt_config(&txconf) != ESP_OK ||
            rmt_driver_install(txconf.channel, 0, 0) != ESP_OK)
        {
            return false;
        }
    }

    // --------------------
    //  CONFIGURE RX
    // --------------------
    if (_rxChannel >= 0) {
        rmt_config_t rxconf = {};
        rxconf.rmt_mode = RMT_MODE_RX;
        rxconf.channel = (rmt_channel_t)_rxChannel;
        rxconf.gpio_num = (gpio_num_t)_rxPin;
        rxconf.mem_block_num = 1;
        rxconf.clk_div = 80;   // 1 tick = 1µs

		rxconf.rx_config.filter_en = false;
        
		/*ALTERNATIVE OPTION FOR AFTER PROFILED NOISE
		rxconf.rx_config.filter_en = true;
        rxconf.rx_config.filter_ticks_thresh = std::max<uint16_t>(1, _periodUs/64);    // ignore <50µs noise
		*/
        rxconf.rx_config.idle_threshold = _periodUs*30;       // break frame
		//^NOT SURE YET WHAT IDEAL VALUE FOR THIS IS^

        if (rmt_config(&rxconf) != ESP_OK ||
            rmt_driver_install(rxconf.channel, RMT_BUFFER_SIZE, 0) != ESP_OK)
        {
            return false;
        }

        rmt_get_ringbuf_handle((rmt_channel_t)_rxChannel, &_rb);
        if (_rb == nullptr) return false;

        rmt_rx_start((rmt_channel_t)_rxChannel, true);
    }

    return true;
}

size_t HardwareRMT::writeBytes(const char* src, size_t len) {
    std::lock_guard<std::mutex> lock(_mutexTX);
	
    if (_txChannel < 0){
		static bool doneWarning = false;
		if(!doneWarning){
			doneWarning = true;
			Log::Warn(TAG, "Trying to writeBytes but TX channel was %d", _txChannel);
		}
		return 0;
	}
	
    size_t nextWriteBufferIndex = 0;
	size_t nWrittenThisWrite = 0;
	size_t currentNWrittenConfirmed = 0;
    for (size_t i = 0; i < len; i++) {
        encodeByte((uint8_t)src[i], _writeBuffer, nextWriteBufferIndex);
		nWrittenThisWrite++;
		if(nextWriteBufferIndex>ITEMS_WRITE_BUFFER_LENGTH-10){
			if(rmt_write_items((rmt_channel_t)_txChannel,
					_writeBuffer, nextWriteBufferIndex, true) != ESP_OK)
			{
				return currentNWrittenConfirmed;
			}
			nextWriteBufferIndex = 0;
			currentNWrittenConfirmed += nWrittenThisWrite;
			nWrittenThisWrite = 0;
		}
    }
	addSyncPulse(_writeBuffer, nextWriteBufferIndex);
	if(rmt_write_items((rmt_channel_t)_txChannel,
				_writeBuffer, nextWriteBufferIndex, true) != ESP_OK)
	{
		return currentNWrittenConfirmed;
	}
    return currentNWrittenConfirmed + nWrittenThisWrite;
}

void HardwareRMT::encodeByte(uint8_t b, rmt_item32_t* items, size_t& nextIndex) {
	addSyncPulse(items, nextIndex);
    for (int bit = 7; bit >= 0; bit--) {
        bool one = (b >> bit) & 0x01;	
		if(one^_invertTx){
			items[nextIndex++]={{ _longPulseUs, 1, _longPulseLowUs, 0 }};
		}
		else{
			items[nextIndex++]={{ _shortPulseUs, 1, _shortPulseLowUs, 0 }};
		}
    }
}
void HardwareRMT::addSyncPulse(rmt_item32_t* items, size_t& nextIndex){
    items[nextIndex++]= {{ _syncPulseUs, 1, _syncPulseLowUs, 0 }};
}

void HardwareRMT::flushTx() {
    if (_txChannel >= 0) {
        rmt_wait_tx_done((rmt_channel_t)_txChannel, portMAX_DELAY);
    }
}
size_t HardwareRMT::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
	if(maxLength<MIN_REQUIRED_RECEIVE_BUFFER_SIZE){
		Aborter::safeAbort(TAG, "The maxLength (given as %zu) must always be greater or equal to MIN_REQUIRED_RECEIVE_BUFFER_SIZE", maxLength);
		return 0;
	}
    if (_rb == nullptr) return 0;
	uint32_t  duration;
	size_t nextDestinationIndex = 0;
	rmt_item32_t* item = nullptr;
	size_t outSize = 0;
	auto* items = (rmt_item32_t*)xRingbufferReceive(
		_rb,
		&outSize,
		pdMS_TO_TICKS(timeoutMs)
	);
	if (!items) return 0;
	size_t nItems = outSize / sizeof(rmt_item32_t);
	size_t nextItemIndex = 0;
	while(nextItemIndex<nItems){
		item = &items[nextItemIndex++];
		duration = item->level0>0?item->duration0:item->duration1;
		if(duration<=_syncPulseMaxUs){
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
			uint8_t bit = ((duration>_shortPulseMaxUs)^_invertRx)?1:0;
			_currentByte = (_currentByte << 1) | bit;
		}
		_nextNBit++;
	}
	vRingbufferReturnItem(_rb, (void*)items);
    return nextDestinationIndex;
}
void HardwareRMT::handleMalformedByte(uint8_t _nextNBit){
	//We dont really need this. Any noise will trigger it. Such as anything at startup*/
	static bool firstMalformedWarning = true;
	if(firstMalformedWarning){
		Log::Warn(TAG, "Received malformed byte with a length of %d", _nextNBit);
		firstMalformedWarning = false;
	}
}

const char* HardwareRMT::getDescription() const {
    return _description;
}
size_t HardwareRMT::getMinRequiredReceiveBufferSize() const{
	return MIN_REQUIRED_RECEIVE_BUFFER_SIZE;
}
