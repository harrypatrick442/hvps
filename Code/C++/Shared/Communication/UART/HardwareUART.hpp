#pragma once
#include "UARTBase.hpp"
#include "driver/uart.h"
class HardwareUART: public UARTBase{
private:
	uart_port_t _uartPort;  // invalid by default
public:
	HardwareUART(
		int nUART,
		int txPin, 
		int rxPin, 
		int baudRate,
		bool invertTx, 
		bool invertRx);
    bool configure(
	) override;
	virtual ~HardwareUART() override;
    size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) override;
    size_t writeBytes(const char* src, size_t len) override;
    void flushTx() override;
};