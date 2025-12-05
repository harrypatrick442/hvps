#pragma once
#include "UARTBase.hpp"
#include "driver/gpio.h"
#include "Core/RingBuffer.hpp"

class SoftwareUART : public UARTBase {
private:
    int _bitPeriodUs;   // derived from baudRate
    int _halfBitPeriodUs;
	gpio_num_t _txGPIONum;
	gpio_num_t _rxGPIONum;
	RingBuffer _rxBuffer;
public:
    SoftwareUART(
        int nUART,
        int txPin,
        int rxPin,
        int baudRate,
        bool invertTx,
        bool invertRx
    );

    bool configure() override;
    size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
    size_t writeBytes(const char* src, size_t len) override;
    void flushTx() override;
	void readLooper();
};
