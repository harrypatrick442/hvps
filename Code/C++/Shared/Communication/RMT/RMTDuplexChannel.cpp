#include "RMTDuplexChannel.hpp"
#include "../Interfaces/IChannel.hpp"
#include <cstring>
const char* RMTDuplexChannel::TAG = "RMTDuplexChannel";
RMTDuplexChannel::RMTDuplexChannel(
	int txChannel,
	int rxChannel,
	int txPin,
	int rxPin,
	int shortPulseUs,
	int longPulseUs
) :DuplexChannel(
	std::unique_ptr<IChannel>(
		new HardwareRMT(txChannel, rxChannel, txPin, rxPin, shortPulseUs, longPulseUs)
	)
)
{
	
}
