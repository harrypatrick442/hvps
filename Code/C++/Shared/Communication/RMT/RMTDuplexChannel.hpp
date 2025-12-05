#pragma once
#include "../../Communication/DuplexChannel.hpp"
#include "HardwareRMT.hpp"
class RMTDuplexChannel : public DuplexChannel{
	public:
		static const char* TAG;
		RMTDuplexChannel(
			int txChannel,
			int rxChannel,
			int txPin,
			int rxPin,
			bool invertTx,
			bool invertRx,
			int periodUs = HardwareRMT::PERIOD_DEFAULT
		);
};