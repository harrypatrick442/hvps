#include "RMTDuplexChannel.hpp"
#include "../Interfaces/IChannel.hpp"
#include <cstring>
const char* RMTDuplexChannel::TAG = "RMTDuplexChannel";
RMTDuplexChannel::RMTDuplexChannel(
	int txPin,
	int rxPin,
	int periodUs,
	bool invertTx,
	bool invertRx
) :DuplexChannel(
	std::unique_ptr<IChannel>(
		new HardwareRMT(txPin, rxPin, periodUs, invertTx, invertRx)
	)
)
{
	
}
