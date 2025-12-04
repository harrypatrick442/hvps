#pragma once
#include "../../Communication/DuplexChannel.hpp"
class UARTDuplexChannel : public DuplexChannel{
	public:
		static const char* TAG;
		UARTDuplexChannel(
			/*uart_port_t*/ int nUART,
			/*int*/ int txPin, 
			/*int*/ int rxPin, 
			/*int*/ int baudRate = 115200,
			bool invertTx = false,
			bool invertRx = false);
};