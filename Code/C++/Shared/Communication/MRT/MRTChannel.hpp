// MRTChannel.hpp
#pragma once
#include "../Interfaces/IChannel.hpp"
#include "MRTSymbol.hpp"
#include "Core/Latch.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include <atomic>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "driver/gptimer.h"
#include "Timing/InterruptTimer.hpp"
// Include FreeRTOS headers for data types
#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h" // <-- Add this line

#ifdef __cplusplus
}
#endif

class MRTChannel : public IChannel {
public:
	static inline constexpr bool INVERT_TX_DEFAULT = false;
	static inline constexpr bool INVERT_RX_DEFAULT = true;
	static inline constexpr int PERIOD_US_DEFAULT = 600;
	static inline constexpr int N_SUB_PULSES_PER_PULSE = 7;
	static inline constexpr int RECEIVE_QUEUE_SIZE_SYMBOLS_DEFAULT = 1000;
	static const char* NOT_INITIALIZED_PROPERLY_MESSAGE;
	
private:
	int _txPin;
	int _rxPin;
	gpio_num_t _txGPIONum;
	gpio_num_t _rxGPIONum;
	bool _invertTx;
	bool _invertRx;
	uint32_t _periodUs;
	uint32_t _syncPulseMinCCycles;
	uint32_t _syncPulseSubPulses;
	uint32_t _syncPulseMaxCCycles;
	uint32_t _zeroPulseSubPulses;
	uint32_t _zeroPulseMaxCCycles;
	uint32_t _onePulseSubPulses;
	uint32_t _onePulseMaxCCycles;
	uint8_t _currentByte;
	uint8_t _nextNBit;
	uint64_t _writeTimerPeriodUs;
	InterruptTimer _writeTimer;
	std::mutex _configureMutex;
	std::mutex _txMutex;
	char _description[32];
	QueueHandle_t _rxSymbolQueue;
	
	volatile MRTSymbol* _symbolsBeingWritten;
	volatile size_t   _symbolsBeingWrittenLength;
	volatile size_t _txISRSymbolIndex = 0;
	volatile int32_t _txISRSubPulsesIntoCurrentSymbol = N_SUB_PULSES_PER_PULSE;
	volatile MRTSymbol _txISRCurrentSymbol;
	Latch _nextWriteBufferForISRFreeLatch;
	
	
	bool _rxHandlerInitialized;
	volatile bool _rxHandlerLastWasHigh;
	volatile uint32_t _rxHandlerHighCCycles;
	std::atomic<bool> _successfullyConfigured;
	std::atomic<bool> _createdRxEdgeInterupt;
public:
	MRTChannel(
		int txPin,
		int rxPin,
		int periodUs = PERIOD_US_DEFAULT,
		bool invertTx = INVERT_TX_DEFAULT,
		bool invertRx = INVERT_RX_DEFAULT,
		size_t receiveQueueSize = RECEIVE_QUEUE_SIZE_SYMBOLS_DEFAULT
	);

	virtual ~MRTChannel();

	bool configure() override;
	void flushTx() override;
	const char* getDescription() const override;
	size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
	size_t writeBytes(const char* src, size_t len) override;void 
	printTimingConfig() const;

private:	

	void addSyncPulse(MRTSymbol* items, size_t& nextSymbolIndex);
	bool configureRx();
	bool configureTx();
	bool configureTxTimer();
	void encodeByte(uint8_t b, MRTSymbol* items, size_t& nextSymbolIndex);
	esp_err_t freeWriteTimerIfCreated();
	void handleMalformedByte(uint8_t _nextNBit);
	void IRAM_ATTR handleTxTickFromISR();
	void IRAM_ATTR handleRxEdgeFromISR();
	uint32_t nSubPulsesToCCycles(int nSubPulses);
	static void IRAM_ATTR rxEdgeISRTrampoline(void* arg);
	void scheduleWriteBuffer(MRTSymbol* symbols, size_t symbolsLength);
	void IRAM_ATTR setIOBasedOnSymbol(int8_t subPulsesIntoCurrentSymbol, MRTSymbol symbol);
	static bool IRAM_ATTR txTimerISRTrampoline(
		void *arg);
};
