#include "MRTDuplexChannel.hpp"
#include "../Interfaces/IChannel.hpp"
#include <cstring>
const char* MRTDuplexChannel::TAG = "MRTDuplexChannel";
MRTDuplexChannel::MRTDuplexChannel(
	int txPin,
	int rxPin,
	int periodUs,
	bool invertTx,
	bool invertRx
) :DuplexChannel(
	std::unique_ptr<IChannel>(
		new MRTChannel(txPin, rxPin, periodUs, invertTx, invertRx)
	)
)
{
	
}
