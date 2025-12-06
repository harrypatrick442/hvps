// HardwareRMT.hpp
#pragma once

#include "../Interfaces/IChannel.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_types.h"
#include "driver/gpio.h"

class HardwareRMT : public IChannel {
private:
	static inline constexpr int RMT_BUFFER_SIZE = 2000;
	static inline constexpr int MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE = 200;
	static inline constexpr int ITEMS_WRITE_BUFFER_LENGTH = 200;
public:
	static inline constexpr bool INVERT_TX_DEFAULT = false;
	static inline constexpr bool INVERT_RX_DEFAULT = true;
	static inline constexpr int PERIOD_US_DEFAULT = 60;
	static inline constexpr const char* TAG = "HardwareRMT";
	static inline constexpr int MIN_N_ITEMS_PER_CHAR = 9;
	static inline constexpr size_t MIN_REQUIRED_RECEIVE_BUFFER_SIZE  = RMT_BUFFER_SIZE/(sizeof(rmt_symbol_word_t)*MIN_N_ITEMS_PER_CHAR);

	HardwareRMT(
		int txChannel,
		int rxChannel,
		int txPin,
		int rxPin,
		int periodUs = PERIOD_US_DEFAULT,
		bool invertTx = INVERT_TX_DEFAULT,
		bool invertRx = INVERT_RX_DEFAULT
	);

	virtual ~HardwareRMT();

	bool configure() override;
	size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
	size_t writeBytes(const char* src, size_t len) override;
	void flushTx() override;
	const char* getDescription() const override;
	size_t getMinRequiredReceiveBufferSize() const override;

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
	u_int8_t _currentByte;
	u_int8_t _nextNBit;
	std::mutex _mutexTX;
	char _description[32];
	rmt_symbol_word_t _writeBuffer[ITEMS_WRITE_BUFFER_LENGTH];

	// --- encoding helpers ---
	void encodeByte(uint8_t b, rmt_symbol_word_t* items, size_t& nextIndex);
	void handleMalformedByte(uint8_t nextNBit);
	void addSyncPulse(rmt_symbol_word_t* items, size_t& nextIndex);
};
