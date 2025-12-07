#include "OutputVoltageFeedbackFiberOpticDuplexChannel.hpp"
#include "PinDefinitions.hpp"
OutputVoltageFeedbackFiberOpticDuplexChannel::OutputVoltageFeedbackFiberOpticDuplexChannel(): TOSLINKDuplexChannel(
	PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_TX,
	PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_RX) {
	
};