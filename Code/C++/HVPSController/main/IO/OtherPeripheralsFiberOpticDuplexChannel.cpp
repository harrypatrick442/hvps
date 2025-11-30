#include "OtherPeripheralsFiberOpticDuplexChannel.hpp"
#include "PinDefinitions.hpp"
//#include "../Generated/HVPSConfig.hpp"
OtherPeripheralsFiberOpticDuplexChannel::OtherPeripheralsFiberOpticDuplexChannel()
: TOSLINKDuplexChannel(
	3,
	PinDefinitions::OTHER_PERIPHERALS_TX,
	PinDefinitions::OTHER_PERIPHERALS_RX,
	4800) {
	
};