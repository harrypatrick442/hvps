// HardwareRMT.hpp
#pragma once
#include "../Interfaces/IChannel.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include <atomic>
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_types.h"
#include "driver/gpio.h"
// Include FreeRTOS headers for data types
#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h" // <-- Add this line

#ifdef __cplusplus
}
#endif

class HardwareRMT : public IChannel {
private:
	static inline constexpr int READ_BUFFER_SIZE_SYMBOLS = 1000;
	static inline constexpr int WRITE_BUFFER_SIZE_SYMBOLS = 200;
	static inline constexpr int RECEIVE_QUEUE_SIZE_SYMBOLS = 1000;
	//static inline constexpr int ITEMS_WRITE_BUFFER_LENGTH = 200;
public:
	static inline constexpr bool INVERT_TX_DEFAULT = false;
	static inline constexpr bool INVERT_RX_DEFAULT = true;
	static inline constexpr int PERIOD_US_DEFAULT = 60;
	static inline constexpr int MIN_N_ITEMS_PER_CHAR = 9;
	static inline constexpr size_t MIN_REQUIRED_RECEIVE_BUFFER_SIZE  = 256;

	HardwareRMT(
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

private:
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
	u_int8_t _currentByte;
	u_int8_t _nextNBit;
	std::atomic<bool> _txChannelCreatedAndEnabled;
	std::atomic<bool> _rxChannelCreatedAndEnabled;
	rmt_transmit_config_t _txTransmitConfig;
	rmt_receive_config_t _rxReceiveConfig;
	std::mutex _mutexTX;
	char _description[32];
	rmt_symbol_word_t _writeBuffer[WRITE_BUFFER_SIZE_SYMBOLS];
	rmt_symbol_word_t _rxBuffer[READ_BUFFER_SIZE_SYMBOLS];
	QueueHandle_t _rxSymbolQueue;
	rmt_channel_handle_t _rxChannelHandle;
	rmt_channel_handle_t _txChannelHandle;
	rmt_encoder_handle_t _txEncoder;
	bool configureRx();
	bool configureTx();
	// --- encoding helpers ---
	void encodeByte(uint8_t b, rmt_symbol_word_t* items, size_t& nextIndex);
	void handleMalformedByte(uint8_t nextNBit);
	void addSyncPulse(rmt_symbol_word_t* items, size_t& index) ;
	static rmt_transmit_config_t createTxConfig();
	static rmt_receive_config_t createRxConfig(int periodUs);
	static bool onReceiveStatic(
		rmt_channel_handle_t channel,
		const rmt_rx_done_event_data_t* edata,
		void* user_ctx
	);
	bool onReceive(const rmt_rx_done_event_data_t* edata);
};
