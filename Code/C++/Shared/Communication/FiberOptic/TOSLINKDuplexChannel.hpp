#pragma once
#include "../RMT/RMTDuplexChannel.hpp"
class TOSLINKDuplexChannel: public RMTDuplexChannel{
public:
	TOSLINKDuplexChannel(
		int txChannel,
		int rxChannel,
		int txPin,
		int rxPin);
};