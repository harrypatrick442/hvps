#include "TOSLINKDuplexChannel.hpp"
#include "../MRT/MRTChannel.hpp"
#include "../UART/HardwareUART.hpp"
#include "System/Aborter.hpp"
#include "../DuplexChannel.hpp"
#include "../Interfaces/IChannel.hpp"
#include <memory>
TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	int txPin,
	int rxPin,
	ChannelType channelType): DuplexChannel(
		channelType==ChannelType::MRT
		?std::unique_ptr<IChannel>(
			new MRTChannel(txPin, rxPin, 400, false, true)
		)
		:std::unique_ptr<IChannel>(
			new HardwareUART(takeNextNUart(), txPin, rxPin, 115200, false, true)
		)
		
	) {
};
int TOSLINKDuplexChannel::takeNextNUart(){
	int nUart = _nextNUart++;
	if(nUart>2){
		Aborter::safeAbort(TAG, "nUart doesnt exist");
	}
	return nUart;
}