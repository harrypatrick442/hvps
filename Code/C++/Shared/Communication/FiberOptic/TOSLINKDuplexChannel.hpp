#pragma once
#include "../DuplexChannel.hpp"
#include "../Enums/ChannelType.hpp"
class TOSLINKDuplexChannel: public DuplexChannel{
private:
	static inline int _nextNUart = 1;
public:
	TOSLINKDuplexChannel(
		int txPin,
		int rxPin,
		ChannelType channelType = ChannelType::MRT);
private:
	static int takeNextNUart();
};