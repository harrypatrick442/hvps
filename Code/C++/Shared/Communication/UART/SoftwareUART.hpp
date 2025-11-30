#pragma once
#include "UARTBase.hpp"
#include "driver/gpio.h"

class SoftwareUART : public UARTBase {
private:
    int _bitPeriodUs;   // derived from baudRate
	gpio_num_t _txGPIONum;
	gpio_num_t _rxGPIONum;
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
    int readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
    int writeBytes(const char* src, size_t len) override;
    void flushTx() override;
};
