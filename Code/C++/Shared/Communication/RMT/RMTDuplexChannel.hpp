#pragma once
#include "../../Communication/DuplexChannel.hpp"
#include "HardwareRMT.hpp"
class RMTDuplexChannel : public DuplexChannel{
	public:
		static const char* TAG;
		RMTDuplexChannel(
			int txPin,
			int rxPin,
			int periodUs = HardwareRMT::PERIOD_US_DEFAULT,
			bool invertTx = HardwareRMT::INVERT_TX_DEFAULT,
			bool invertRx = HardwareRMT::INVERT_RX_DEFAULT
		);
};