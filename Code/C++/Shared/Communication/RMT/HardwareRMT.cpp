#include "HardwareRMT.hpp"
#include "System/Aborter.hpp"
#include <cstdio>
#include <cstring>

/*
                   ONE FULL FRAME (1 BYTE)
              -------------------------------------

                   HIGH=meaningful    LOW=gap

            ┌────────────────────────────────────────┐
            │              HIGH Pulses               │
            └────────────────────────────────────────┘

start bit  data bit 7  data bit 6 ... data bit 0   stop bit
─────────┬──────────┬──────────┬───────┬──────────┬─────────

HIGH pulse:

     SHORT         LONG or SHORT         LONG
  (start mark)      (bit0 or bit1)      (stop mark)
  duration ≤ _startPulseMaxUs
                   duration <  _middlePulseUs  → ‘0’
                   duration >= _middlePulseUs  → ‘1’
                                            duration >= _stopPulseMinUs


ASCII timeline (not drawn to scale, but structured):

    TIME →
    ┌──────────────────────────────────────────────────────────────┐
    │                                                              │
    │   START     BIT7      BIT6     …     BIT1      BIT0     STOP│
    │                                                              │
    └──────────────────────────────────────────────────────────────┘

    HIGH pulse:
    ┌───────┐  ┌──────────┐  ┌────┐        ┌──────────┐  ┌───────────────┐
    │       │  │          │  │    │        │          │  │               │
    │ SHORT │  │ SHORT/   │  │... │  ...   │ SHORT/   │  │    LONG       │
    │  (S)  │  │  LONG    │  │    │        │  LONG    │  │   (STOP)      │
    └───────┘  └──────────┘  └────┘        └──────────┘  └───────────────┘
      ≤ _startBitMaxUs      < or ≥ _middlePulseUs     ≥ _stopPulseMinUs

    LOW gap after each symbol:
            ┌────┐    ┌────┐    ┌────┐ ...    ┌────┐    ┌────┐
            │    │    │    │    │    │        │    │    │    │
            │ LOW│    │LOW │    │LOW │        │LOW │    │LOW │
            └────┘    └────┘    └────┘        └────┘    └────┘


Legend:
    S  = start pulse (very short)
    0  = data bit 0  (short pulse)
    1  = data bit 1  (long pulse)
    STOP = long pulse marking byte termination

This is what happens when the best of organic developers meet the best of AI developers :)
*/
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
    _shortPulseUs((periodUs*3)/8),
    _shortPulseLowUs(periodUs - _shortPulseUs),
    _longPulseUs((periodUs*5)/8),
    _longPulseLowUs(periodUs - _longPulseUs),
	_middlePulseUs(periodUs/2),
	_startPulseUs(periodUs/8),
	_startPulseLowUs(periodUs - _startPulseUs),
	_stopPulseUs((periodUs*7)/8),
	_stopPulseLowUs(periodUs - _stopPulseUs),
	_stopPulseUs((periodUs*7)/8),
	_startPulseMaxUs(periodUs/4),
	_stopPulseMinUs((periodUs*3)/4),
    _rb(nullptr)
{
	if(_shortPulseUs>=_longPulseUs){
		Aborter::safeAbort(TAG, "_shortPulseUs must be less than _longPulseUs");
	}
	if(_middlePulseUs==_shortPulseUs||_middlePulseUs==_longPulseUs){
		Aborter::safeAbort(TAG, "_middlePulseUs was computed to be the same as _shortPulseUs or _longPulseUs! You need the long and short pulse to have more space between them");
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

        rxconf.rx_config.filter_en = true;
        rxconf.rx_config.filter_ticks_thresh = 50;    // ignore <50µs noise
        rxconf.rx_config.idle_threshold = 1000;       // break frame

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

int HardwareRMT::writeBytes(const char* src, size_t len) {
    std::lock_guard<std::mutex> lock(_mutexTX);

    if (_txChannel < 0) return -1;
	
    std::vector<rmt_item32_t> items;
    items.reserve(len * 10);
    for (size_t i = 0; i < len; i++) {
        encodeByte((uint8_t)src[i], items);
    }
    if (rmt_write_items((rmt_channel_t)_txChannel,
            items.data(), items.size(), true) != ESP_OK)
    {
        return -1;
    }
    return (int)len;
}

void HardwareRMT::encodeByte(uint8_t b, std::vector<rmt_item32_t>& items) {
    items.push_back({{ _startPulseUs, 1, _startPulseLowUs, 0 }});
    for (int bit = 7; bit >= 0; bit--) {
        bool one = (b >> bit) & 0x01;	
		if(one^_invertTx){
			items.push_back({{ _longPulseUs, 1, _longPulseLowUs, 0 }});
		}
		else{
			items.push_back({{ _shortPulseUs, 1, _shortPulseLowUs, 0 }});
		}
    }
    items.push_back({{ _stopPulseUs, 1, _stopPulseLowUs, 0 }});
}

void HardwareRMT::flushTx() {
    if (_txChannel >= 0) {
        rmt_wait_tx_done((rmt_channel_t)_txChannel, portMAX_DELAY);
    }
}
int HardwareRMT::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
    if (_rb == nullptr) return 0;

	uint8_t currentByte = 0;
	uint8_t nextNBit = 0;
	bool doneWithCurrentByte = false;
	uint32_t  duration;
	size_t nextDestinationIndex = 0;
	rmt_item32_t* item = nullptr;
	
    while (true) {
		size_t maxNItemsRetrieve = maxLength-nextDestinationIndex;
		if(maxNItemsRetrieve<=0){
			break;
		}
		if(maxNItemsRetrieve>MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE){
			maxNItemsRetrieve = MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE;
		}
        size_t outSize = 0;
        auto* items = (rmt_item32_t*)xRingbufferReceiveUpTo(
            _rb,
            &outSize,
            pdMS_TO_TICKS(timeoutMs),
			maxNItemsRetrieve
        );
        if (!items) break;
        size_t nItems = outSize / sizeof(rmt_item32_t);
		size_t nextItemIndex = 0;
		while(nextItemIndex<nItems){
			item = &items[nextItemIndex++];
			duration = item->level0>0?item->duration0:item->duration1;
			if(duration<=_startPulseMaxUs){
				nextNBit = 0;
				doneWithCurrentByte = false;
				currentByte = 0;
				continue;
			}
			if(duration >=_stopPulseMinUs){
				if(nextNBit<9){
					handleMalformed(currentByte);
					continue;
				}
				continue;
			}
			if(doneWithCurrentByte){
				continue;
			}
			uint8_t bit = ((duration>=_middlePulseUs)^_invertRx)?1:0;
            currentByte = (currentByte << 1) | bit;
			if(nextNBit>=8){
				doneWithCurrentByte = true;
				destination[nextDestinationIndex++]=currentByte;
				if(nextDestinationIndex>=maxLength){
					break;
				}
				nextNBit = 0;
				continue;
			}
            nextNBit++;
        }
        vRingbufferReturnItem(_rb, (void*)items);
    }

    return nextDestinationIndex;
}

const char* HardwareRMT::getDescription() const {
    return _description;
}
