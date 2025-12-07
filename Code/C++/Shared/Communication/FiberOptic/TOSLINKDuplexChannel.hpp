#pragma once
#include "../RMT/RMTDuplexChannel.hpp"
class TOSLINKDuplexChannel: public RMTDuplexChannel{
public:
	TOSLINKDuplexChannel(
		int txPin,
		int rxPin);
};