#pragma once

#include "../Interfaces/IChannel.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include "driver/rmt.h"
#include "driver/gpio.h"

class HardwareRMT : public IChannel {
private:
	static inline constexpr int RMT_BUFFER_SIZE = 2000;
	static inline constexpr int MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE = 200;
	static inline constexpr int ITEMS_WRITE_BUFFER_LENGTH = 200;
public:
	static inline constexpr int PERIOD_DEFAULT = 20;
	static inline constexpr const char* TAG = "HardwareRMT";
    HardwareRMT(
        int txChannel,
        int rxChannel,
        int txPin,
        int rxPin,
        int periodUs = PERIOD_DEFAULT,
		bool invertTx = false,
		bool invertRx = false
    );

    virtual ~HardwareRMT();

    bool configure() override;
    size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
    size_t writeBytes(const char* src, size_t len) override;
    void flushTx() override;
    const char* getDescription() const override;

private:
    int _txChannel;
    int _rxChannel;
    int _txPin;
    int _rxPin;
	bool _invertTx;
	bool _invertRx;
	uint32_t _periodUs;
	uint32_t _syncPulseUs;
	uint32_t _syncPulseLowUs;
	uint32_t _syncPulseMaxUs;
    uint32_t _shortPulseUs;
    uint32_t _shortPulseLowUs;
    uint32_t _shortPulseMaxUs;
    uint32_t _longPulseUs;
    uint32_t _longPulseLowUs;
    RingbufHandle_t _rb;
    std::mutex _mutexTX;
    char _description[32];
	rmt_item32_t _writeBuffer[ITEMS_WRITE_BUFFER_LENGTH];
    // --- encoding helpers ---
    void encodeByte(uint8_t b, rmt_item32_t* items, size_t& nextIndex);
	void handleMalformedByte(uint8_t nextNBit);
	void addSyncPulse(rmt_item32_t* items, size_t& nextIndex);
};
