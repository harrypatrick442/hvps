#include "FiberOpticDuplexChannel_1.hpp"
#include "PinDefinitions.hpp"
FiberOpticDuplexChannel_1::FiberOpticDuplexChannel_1(): TOSLINKDuplexChannel(
	0, 1, PinDefinitions::TX, PinDefinitions::RX) {
	
};