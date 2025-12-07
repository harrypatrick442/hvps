#include "FirstStageVoltageFeedbackFiberOpticDuplexChannel.hpp"
#include "PinDefinitions.hpp"
FirstStageVoltageFeedbackFiberOpticDuplexChannel::FirstStageVoltageFeedbackFiberOpticDuplexChannel(): TOSLINKDuplexChannel(
	PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_TX,
	PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_RX) {
	
};