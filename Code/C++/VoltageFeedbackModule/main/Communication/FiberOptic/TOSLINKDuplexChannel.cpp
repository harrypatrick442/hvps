#include "TOSLINKDuplexChannel.hpp"

TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	/*uart_port_t*/ int nUART,
	/*int*/ int txPin, 
	/*int*/ int rxPin, 
	/*int*/ int baudRate): UARTDuplexChannel(nUART, txPin, rxPin, baudRate, false, true) {
};