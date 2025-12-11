#pragma once
#include "../MRT/MRTDuplexChannel.hpp"
class TOSLINKDuplexChannel: public MRTDuplexChannel{
public:
	TOSLINKDuplexChannel(
		int txPin,
		int rxPin);
};