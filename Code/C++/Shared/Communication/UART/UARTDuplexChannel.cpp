#include "UARTDuplexChannel.hpp"
#include "HardwareUART.hpp"
#include "SoftwareUART.hpp"
#include "../Interfaces/IChannel.hpp"
#include <cstring>
const char* UARTDuplexChannel::TAG = "UARTDuplexChannel";
UARTDuplexChannel::UARTDuplexChannel(
	/*uart_port_t*/ int nUART,
	/*int*/ int txPin, 
	/*int*/ int rxPin, 
	/*int*/ int baudRate,
	bool invertTx,
	bool invertRx
) :DuplexChannel((nUART < 3)
		? std::unique_ptr<IChannel>(
			new HardwareUART(nUART, txPin, rxPin, baudRate, invertTx, invertRx)
		)
		: std::unique_ptr<IChannel>(
			new SoftwareUART(nUART, txPin, rxPin, baudRate, invertTx, invertRx)
		)
)
{
	
}
