#ifndef TOSLINKDuplexChannel_hpp
#define TOSLINKDuplexChannel_hpp
#include "../UART/UARTDuplexChannel.hpp"
class TOSLINKDuplexChannel: public UARTDuplexChannel{
	public:
        TOSLINKDuplexChannel(
			/*uart_port_t*/ int nUART,
			/*int*/ int txPin, 
			/*int*/ int rxPin, 
			/*int*/ int baudRate = 19200);
};
#endif // TOSLINKDuplexChannel_hpp