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
			int shortPulseUs = HardwareRMT::SHORT_PULSE_US_DEFAULT,
			int longPulseUs = HardwareRMT::LONG_PULSE_US_DEFAULT
		);
};