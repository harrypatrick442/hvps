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
	_nextNBit(0),
	_txChannelCreatedAndEnabled(false),
	_rxChannelCreatedAndEnabled(false)
{
    std::snprintf(_description, sizeof(_description),
        "RMT(tx:%d rx:%d)", txChannel, rxChannel);
	if (periodUs < 6) {
		Aborter::safeAbort(TAG, "periodUs too small for pulse scheme");
		return;
	}
    if (_rxChannel < 0) {
		Aborter::safeAbort(TAG, "You provided rxChannel < 0");
		return;
	}
    if (_txChannel < 0) {
		Aborter::safeAbort(TAG, "You provided txChannel < 0");
		return;
	}
}

HardwareRMT::~HardwareRMT() {
    if (_txChannelCreatedAndEnabled) {
		rmt_disable((rmt_channel_handle_t)_txChannel);
        rmt_del_channel((rmt_channel_t)_txChannel);
    }
    if (_rxChannel >= 0) {
		rmt_disable((rmt_channel_handle_t)_rxChannel);
        rmt_del_channel((rmt_channel_t)_rxChannel);
    }
}

bool HardwareRMT::configure() {
	if(!configureRx()){
		return false;
	}
    return configureTx();
}
bool HardwareRMT::configureRx() {
    if (_rxChannel < 0) {
        return false;
    }

    rmt_rx_channel_config_t rx_config = {
        .gpio_num = (gpio_num_t)_rxPin,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,
        .mem_block_symbols = READ_BUFFER_SIZE_SYMBOLS
		// Do NOT set .invert_in here — handled explicitly in readBytes() for clarity and consistency and it makes the code simpler.
		.flags = {
			.invert_in = false
		}
    };

    esp_err_t res = rmt_new_rx_channel(&rx_config, (rmt_channel_handle_t*)&_rxChannel);
    if (res != ESP_OK) {
        Aborter::safeAbort(
            TAG,
            "Failed to create RX RMT channel (pin=%d, res=%d, err='%s')",
            _rxPin, res, esp_err_to_name(res)
        );
        return false;
    }

    rmt_receive_config_t recv_cfg = {
        .signal_range_min_ns = 500,
        .signal_range_max_ns = _periodUs * 1000 * 30,
    };

    res = rmt_rx_register_event_queue((rmt_channel_handle_t)_rxChannel, &_rxQueue, RX_EVENT_QUEUE_LENGTH);
    if (res != ESP_OK) {
        rmt_del_channel((rmt_channel_handle_t)_rxChannel);  // cleanup
        _rxChannel = -1;
        Aborter::safeAbort(
            TAG,
            "Failed to register RX event queue (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }

    res = rmt_enable((rmt_channel_handle_t)_rxChannel);
    if (res != ESP_OK) {
        rmt_del_channel((rmt_channel_handle_t)_rxChannel);  // cleanup
        _rxChannel = -1;
        Aborter::safeAbort(
            TAG,
            "Failed to enable RX RMT channel (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }

    res = rmt_receive((rmt_channel_handle_t)_rxChannel, &_rxQueue, &recv_cfg);
    if (res != ESP_OK) {
        rmt_disable((rmt_channel_handle_t)_rxChannel);      // disable before deleting
        rmt_del_channel((rmt_channel_handle_t)_rxChannel);  // cleanup
        _rxChannel = -1;
        Aborter::safeAbort(
            TAG,
            "Failed to start RX receiving (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }
	_rxChannelCreatedAndEnabled = true;
    return true;
}
bool HardwareRMT::configureTx() {
	if (_txChannel < 0) {
		return false;
	}
    rmt_tx_channel_config_t tx_config = {
        .gpio_num = (gpio_num_t)_txPin,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,  // 1us resolution
        .mem_block_symbols = WRITE_BUFFER_SIZE_SYMBOLS,
        .trans_queue_depth = 4,
		//DO NOT INVERT HERE. ITS DONE IN WRITE. INVERTING AT THIS LEVEL OVERCOMPLICATES THINGS.
        .flags = {
            .invert_out = false
        }
    };

    esp_err_t res = rmt_new_tx_channel(&tx_config, (rmt_channel_handle_t*)&_txChannel);
    if (res != ESP_OK) {
        Aborter::safeAbort(
            TAG,
            "Failed to create TX RMT channel (pin=%d, res=%d, err='%s')",
            _txPin, res, esp_err_to_name(res)
        );
        _txChannel = -1;
        return false;
    }

    res = rmt_enable((rmt_channel_handle_t)_txChannel);
    if (res != ESP_OK) {
        rmt_del_channel((rmt_channel_handle_t)_txChannel);  // cleanup
        _txChannel = -1;
        Aborter::safeAbort(
            TAG,
            "Failed to enable TX RMT channel (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }

	_txChannelCreatedAndEnabled = true;
    return true;
}

size_t HardwareRMT::writeBytes(const char* src, size_t len) {
    if (_txChannel < 0){
		return 0;
	}
    std::lock_guard<std::mutex> lock(_mutexTX);
    size_t nextWriteBufferIndex = 0;
	size_t nCharsWrittenThisWrite = 0;
	size_t currentNCharsWrittenConfirmed = 0;
	exp_err_t err;
    for (size_t i = 0; i < len; i++) {
        encodeByte(static_cast<uint8_t>(src[i]), _writeBuffer, nextWriteBufferIndex);
		nCharsWrittenThisWrite++;
		if(nextWriteBufferIndex>WRITE_BUFFER_SIZE_SYMBOLS-10){
			err = rmt_transmit((rmt_channel_handle_t)_txChannel, _writeBuffer, nextWriteBufferIndex, true);
			if(err != ESP_OK)
			{
				return currentNCharsWrittenConfirmed;
			}
			nextWriteBufferIndex = 0;
			currentNCharsWrittenConfirmed += nCharsWrittenThisWrite;
			nCharsWrittenThisWrite = 0;
		}
    }
	addSyncPulse(_writeBuffer, nextWriteBufferIndex);
    err = rmt_transmit((rmt_channel_handle_t)_txChannel, _writeBuffer, nextWriteBufferIndex, true);
	if(err != ESP_OK)
	{
		return currentNCharsWrittenConfirmed;
	}
    return currentNCharsWrittenConfirmed  + nCharsWrittenThisWrite;
}

void HardwareRMT::encodeByte(uint8_t b, rmt_symbol_word_t* items, size_t& nextIndex) {
	addSyncPulse(items, nextIndex);
    for (int bit = 7; bit >= 0; bit--) {
        bool one = (b >> bit) & 0x01;	
		if(one^_invertTx){
			items[nextIndex++] = {
				.level0 = 1,
				.duration0 = static_cast<uint16_t>(_longPulseUs),
				.level1 = 0,
				.duration1 = static_cast<uint16_t>(_longPulseLowUs)
			};
		}
		else{
			items[nextIndex++] = {
				.level0 = 1,
				.duration0 = static_cast<uint16_t>(_shortPulseUs),
				.level1 = 0,
				.duration1 = static_cast<uint16_t>(_shortPulseLowUs)
			};
		}
    }
}
void HardwareRMT::encodeByte(uint8_t b, rmt_symbol_word_t* items, size_t& index) {
    addSyncPulse(items, index);
    for (int i = 7; i >= 0; --i) {
        bool bit = ((b >> i) & 1) ^ _invertTx;
        items[index++] = {
            .level0 = 1,
            .duration0 = static_cast<uint16_t>(bit ? _longPulseUs : _shortPulseUs),
            .level1 = 0,
            .duration1 = static_cast<uint16_t>(bit ? _longPulseLowUs : _shortPulseLowUs),
        };
    }
}

void HardwareRMT::addSyncPulse(rmt_symbol_word_t* items, size_t& index) {
    items[index++] = {
        .level0 = 1,
        .duration0 = static_cast<uint16_t>(_syncPulseUs),
        .level1 = 0,
        .duration1 = static_cast<uint16_t>(_syncPulseLowUs),
    };
}

void HardwareRMT::flushTx() {
	//rmt_tx_wait_all_done((rmt_channel_handle_t)_txChannel, portMAX_DELAY);
	//Not needed due to true in rmt_transmit
}
size_t HardwareRMT::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
    if (!_rxQueue) return 0;
	if(maxLength<MIN_REQUIRED_RECEIVE_BUFFER_SIZE){
		Aborter::safeAbort(TAG, "The maxLength (given as %zu) must always be greater or equal to MIN_REQUIRED_RECEIVE_BUFFER_SIZE", maxLength);
		return 0;
	}
    rmt_rx_event_data_t* item = nullptr;
    if (xQueueReceive(_rxQueue, &item, pdMS_TO_TICKS(timeoutMs)) != pdTRUE || item == nullptr){
        return 0;
	}
	size_t nextItemIndex = 0;
	size_t nextDestinationIndex = 0;
	while(nextItemIndex<item->num_symbols){
        auto symbol = item->symbols[nextItemIndex++];
		uint32_t duration = symbol.level0>0?symbol.duration0:symbol.duration1;
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
