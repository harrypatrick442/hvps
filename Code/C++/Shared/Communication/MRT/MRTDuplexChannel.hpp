#pragma once
#include "../../Communication/DuplexChannel.hpp"
#include "MRTChannel.hpp"
class MRTDuplexChannel : public DuplexChannel{
	public:
		static const char* TAG;
		MRTDuplexChannel(
			int txPin,
			int rxPin,
			int periodUs = MRTChannel::PERIOD_US_DEFAULT,
			bool invertTx = MRTChannel::INVERT_TX_DEFAULT,
			bool invertRx = MRTChannel::INVERT_RX_DEFAULT
		);
};