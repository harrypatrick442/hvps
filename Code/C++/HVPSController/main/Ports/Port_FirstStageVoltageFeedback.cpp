#include "Port_FirstStageVoltageFeedback.hpp"
#include "../IO/FirstStageVoltageFeedbackFiberOpticDuplexChannel.hpp"
#include "../Communication/FiberOptic/TOSLINKDuplexChannel.hpp"

Port_FirstStageVoltageFeedback& Port_FirstStageVoltageFeedback::initialize() noexcept{
	return SingletonBase<Port_FirstStageVoltageFeedback>::initialize(
		new FirstStageVoltageFeedbackFiberOpticDuplexChannel()
	);
}
Port_FirstStageVoltageFeedback::Port_FirstStageVoltageFeedback(TOSLINKDuplexChannel* toslinkDuplexChannel)
:Port_VoltageFeedbackBase(toslinkDuplexChannel){
}
