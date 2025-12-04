#include "FiberOpticDuplexChannel_1.hpp"
#include "PinDefinitions.hpp"
FiberOpticDuplexChannel_1::FiberOpticDuplexChannel_1(): TOSLINKDuplexChannel(
	1, 2, PinDefinitions::TX, PinDefinitions::RX, 300) {
	
};