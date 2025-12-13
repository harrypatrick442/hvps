#include "FiberOpticDuplexChannel_1.hpp"
#include "Communication/Enums/ChannelType.hpp"

FiberOpticDuplexChannel_1::FiberOpticDuplexChannel_1(): TOSLINKDuplexChannel(
	23, 
	22,
	ChannelType::UART) {
	
};