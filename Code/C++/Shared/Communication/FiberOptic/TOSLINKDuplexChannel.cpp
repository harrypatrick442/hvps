#include "TOSLINKDuplexChannel.hpp"
#include "../MRT/MRTChannel.hpp"
#include "../UART/HardwareUART.hpp"
#include "System/SafeAbort.hpp"
#include "../DuplexChannel.hpp"
#include "../Interfaces/IChannel.hpp"
#include <memory>
TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	int txPin,
	int rxPin,
	ChannelType channelType): DuplexChannel(
		channelType==ChannelType::MRT
		?std::unique_ptr<IChannel>(
			new MRTChannel(txPin, rxPin, 2000, false, true)
		)
		:std::unique_ptr<IChannel>(
			new HardwareUART(takeNextNUart(), txPin, rxPin, 9600, false, true)
		)
		
	) {
};
int TOSLINKDuplexChannel::takeNextNUart(){
	int nUart = _nextNUart++;
	if(nUart>2){
		SAFE_ABORT("nUart doesnt exist");
	}
	return nUart;
}