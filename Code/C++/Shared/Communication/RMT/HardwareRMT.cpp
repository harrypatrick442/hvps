#include "HardwareRMT.hpp"
#include "System/SafeAbort.hpp"
#include "Logging/Log.hpp"
#include <cstdio>
#include <cstring>
HardwareRMT::HardwareRMT(
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
	_syncPulseUs((periodUs*1)/6),
	_syncPulseLowUs(periodUs - _syncPulseUs),
	_syncPulseMaxUs((periodUs*2)/6),
    _shortPulseUs((periodUs*3)/6),
    _shortPulseLowUs(periodUs - _shortPulseUs),
	_shortPulseMaxUs((periodUs*4)/6),
    _longPulseUs((periodUs*5)/6),
    _longPulseLowUs(periodUs - _longPulseUs),
	_currentByte(0),
	_nextNBit(0),
	_txChannelCreatedAndEnabled(false),
	_rxChannelCreatedAndEnabled(false),
	_txTransmitConfig(createTxConfig()),
    _rxReceiveConfig(createRxConfig(periodUs)),
	_rxSymbolQueue(nullptr)
{
    std::snprintf(_description, sizeof(_description),
        "HardwareRMT");
	if (periodUs < 6) {
		SAFE_ABORT("periodUs too small for pulse scheme");
		return;
	}
	_rxSymbolQueue = xQueueCreate(
        RECEIVE_QUEUE_SIZE_SYMBOLS,                        // number of items
        sizeof(rmt_symbol_word_t)    // each item size
    );
	if (!_rxSymbolQueue) {
		SAFE_ABORT("Failed to create RX symbol queue");
		return;
	}
	memset(_rxBuffer, 0, sizeof(_rxBuffer));

}

HardwareRMT::~HardwareRMT() {
    if (_txChannelCreatedAndEnabled.load()) {
		rmt_disable(_txChannelHandle);
        rmt_del_channel(_txChannelHandle);
    }
    if (_rxChannelCreatedAndEnabled.load()) {
		rmt_disable(_rxChannelHandle);
        rmt_del_channel(_rxChannelHandle);
    }
}

bool HardwareRMT::configure() {
	LOG_INFO("configure called");
    if(!configureTx()){
		LOG_INFO("failed Tx");
		return false;
	}
	LOG_INFO("did Tx");
	if(!configureRx()){
		LOG_INFO("failed Rx");
		return false;
	}
	LOG_INFO("did Rx");
	return true;
}
bool HardwareRMT::configureRx() {
    rmt_rx_channel_config_t rx_config = {
        .gpio_num = (gpio_num_t)_rxPin,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,
        .mem_block_symbols = READ_BUFFER_SIZE_SYMBOLS
		// Do NOT set .invert_in here — handled explicitly in readBytes() for clarity and consistency and it makes the code simpler.
    };
	rx_config.flags = {
		.invert_in = false
	};
    esp_err_t res = rmt_new_rx_channel(&rx_config, /*(rmt_channel_handle_t*)*/&_rxChannelHandle);
    if (res != ESP_OK) {
        SAFE_ABORT(
            "Failed to create RX RMT channel (pin=%d, res=%d, err='%s')",
            _rxPin, res, esp_err_to_name(res)
        );
        return false;
    }

	rmt_rx_event_callbacks_t cbs = {
		.on_recv_done = &HardwareRMT::onReceiveStatic
	};
	res = rmt_rx_register_event_callbacks(_rxChannelHandle, &cbs, this);
    if (res != ESP_OK) {
        rmt_del_channel(_rxChannelHandle);
        SAFE_ABORT(
            "Failed to register RX event queue (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }

    res = rmt_enable(_rxChannelHandle);
    if (res != ESP_OK) {
        rmt_del_channel(_rxChannelHandle);
        SAFE_ABORT(
            "Failed to enable RX RMT channel (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }
    res = rmt_receive(_rxChannelHandle, &_rxBuffer, READ_BUFFER_SIZE_SYMBOLS, &_rxReceiveConfig);
    if (res != ESP_OK) {
        rmt_disable(_rxChannelHandle);
        rmt_del_channel(_rxChannelHandle);
        SAFE_ABORT(
            "Failed to start RX receiving (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }
	_rxChannelCreatedAndEnabled = true;
    return true;
}
bool HardwareRMT::configureTx() {

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

    esp_err_t res = rmt_new_tx_channel(&tx_config, &_txChannelHandle);
    if (res != ESP_OK) {
        SAFE_ABORT(
            "Failed to create TX RMT channel (pin=%d, res=%d, err='%s')",
            _txPin, res, esp_err_to_name(res)
        );
        return false;
    }

    res = rmt_enable(_txChannelHandle);
    if (res != ESP_OK) {
        rmt_del_channel(_txChannelHandle);
        SAFE_ABORT(
            "Failed to enable TX RMT channel (res=%d, err='%s')",
            res, esp_err_to_name(res)
        );
        return false;
    }
	
	rmt_copy_encoder_config_t enc_cfg = {};
	res = rmt_new_copy_encoder(&enc_cfg, &_txEncoder);
	if (res != ESP_OK) {
		rmt_disable(_txChannelHandle);
        rmt_del_channel(_txChannelHandle);
		SAFE_ABORT("Failed to create copy encoder (res=%d, '%s')",
						   res, esp_err_to_name(res));
		return false;
	}
	_txChannelCreatedAndEnabled = true;
    return true;
}

size_t HardwareRMT::writeBytes(const char* src, size_t len) {
    std::lock_guard<std::mutex> lock(_mutexTX);
    size_t nextWriteBufferIndex = 0;
	size_t nCharsWrittenThisWrite = 0;
	size_t currentNCharsWrittenConfirmed = 0;
	esp_err_t err;
    for (size_t i = 0; i < len; i++) {
        encodeByte(static_cast<uint8_t>(src[i]), _writeBuffer, nextWriteBufferIndex);
		nCharsWrittenThisWrite++;
		if(nextWriteBufferIndex>WRITE_BUFFER_SIZE_SYMBOLS-10){
			err = rmt_transmit(_txChannelHandle, _txEncoder, _writeBuffer, nextWriteBufferIndex, &_txTransmitConfig);
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
    err = rmt_transmit(_txChannelHandle, _txEncoder, _writeBuffer, nextWriteBufferIndex, &_txTransmitConfig);
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
				.duration0 = static_cast<uint16_t>(_longPulseUs),
				.level0 = 1,
				.duration1 = static_cast<uint16_t>(_longPulseLowUs),
				.level1 = 0
			};
		}
		else{
			items[nextIndex++] = {
				.duration0 = static_cast<uint16_t>(_shortPulseUs),
				.level0 = 1,
				.duration1 = static_cast<uint16_t>(_shortPulseLowUs),
				.level1 = 0
			};
		}
    }
}

void HardwareRMT::addSyncPulse(rmt_symbol_word_t* items, size_t& index) {
    items[index++] = {
        .duration0 = static_cast<uint16_t>(_syncPulseUs),
        .level0 = 1,
        .duration1 = static_cast<uint16_t>(_syncPulseLowUs),
        .level1 = 0
    };
}

void HardwareRMT::flushTx() {
	//rmt_tx_wait_all_done((rmt_channel_handle_t)_txChannel, portMAX_DELAY);
	//Not needed due to true in rmt_transmit
}
size_t HardwareRMT::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
    if (!_rxSymbolQueue) return 0;
	size_t nextDestinationIndex = 0;
	rmt_symbol_word_t symbol;
	while(nextDestinationIndex<maxLength){
		if (xQueueReceive(_rxSymbolQueue, &symbol, pdMS_TO_TICKS(timeoutMs)) != pdTRUE) {
			break;
		}
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
	static bool _firstMalformedWarning = true;
	if(_firstMalformedWarning){
		LOG_WARN("Received malformed byte with a length of %d", _nextNBit);
		_firstMalformedWarning = false;
	}
}

const char* HardwareRMT::getDescription() const {
    return _description;
}
rmt_transmit_config_t HardwareRMT::createTxConfig() {
    rmt_transmit_config_t s;
    s.loop_count = 0;
	s.flags = { .eot_level = 0, .queue_nonblocking = 0 };
	return s;
}
rmt_receive_config_t  HardwareRMT::createRxConfig(int periodUs) {
    rmt_receive_config_t s;
	
	s.signal_range_min_ns = 500;/*This is the minimum duration that the RMT will consider as a valid symbol.
	half a micro second because the shortest period i allow as a parameter is 1us. Could make longer but safe for now*/
    s.signal_range_max_ns = periodUs * 1200;
	
	/*This is the maximum duration the RMT will treat as a normal symbol before declaring an idle / end-of-packet event.
	1000 for nano seconds, 1.2 so dont confuse full length as declaring an idle.*/
	
	//s.extra_rmt_receive_flags = {.en_partial_rx = 1};
	s.flags.en_partial_rx = 1;
	/*Enables partial receive events — i.e., RMT will fire callbacks even when only half the buffer
	is filled (or when idle is detected), instead of waiting for the buffer to completely fill.
	This is the setting that enables streaming / continuous RX mode.
	*/
	return s;
}
bool HardwareRMT::onReceiveStatic(
    rmt_channel_handle_t channel,
    const rmt_rx_done_event_data_t* edata,
    void* user_ctx
) {
    return static_cast<HardwareRMT*>(user_ctx)->onReceive(edata);
}
bool HardwareRMT::onReceive(const rmt_rx_done_event_data_t* edata) {
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
