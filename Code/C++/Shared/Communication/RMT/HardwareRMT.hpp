#pragma once

#include "../Interfaces/IChannel.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include "driver/rmt.h"
#include "driver/gpio.h"

class HardwareRMT : public IChannel {
public:
	static constexpr int SHORT_PULSE_US_DEFAULT = 10;
	static constexpr int LONG_PULSE_US_DEFAULT = 20;
	static constexpr int RMT_BUFFER_SIZE = 2000;
	static constexpr int MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE = 200;
    HardwareRMT(
        int txChannel,
        int rxChannel,
        int txPin,
        int rxPin,
        int shortPulseUs = SHORT_PULSE_US_DEFAULT,
		int longPulseUs = LONG_PULSE_US_DEFAULT,
		bool invertTx = false,
		bool invertRx = false
    );

    virtual ~HardwareRMT();

    bool configure() override;
    int readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
    int writeBytes(const char* src, size_t len) override;
    void flushTx() override;
    const char* getDescription() const override;

private:
    int _txChannel;
    int _rxChannel;
    int _txPin;
    int _rxPin;
    int _shortPulseUs;
    int _longPulseUs;
	bool _invertTx;
	bool _invertRx;
	int _middlePulseUs;
    RingbufHandle_t _rb;
    std::mutex _mutexTX;
    char _description[32];

    // --- encoding helpers ---
    void encodeByte(uint8_t b, std::vector<rmt_item32_t>& items);
};
