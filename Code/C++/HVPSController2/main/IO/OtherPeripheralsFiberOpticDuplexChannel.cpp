#include "OtherPeripheralsFiberOpticDuplexChannel.hpp"
#include "PinDefinitions.hpp"
#include "Communication/Enums/ChannelType.hpp"
OtherPeripheralsFiberOpticDuplexChannel::OtherPeripheralsFiberOpticDuplexChannel()
: TOSLINKDuplexChannel(
		PinDefinitions::OTHER_PERIPHERALS_TX,
		PinDefinitions::OTHER_PERIPHERALS_RX,
		ChannelType::MRT
	) {
	
};