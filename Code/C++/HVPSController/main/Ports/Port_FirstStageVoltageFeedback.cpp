#include "Port_FirstStageVoltageFeedback.hpp"
#include "../IO/FirstStageVoltageFeedbackFiberOpticDuplexChannel.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Macros/GetFileName.hpp"
const char* Port_FirstStageVoltageFeedback::getTag() {return GET_FILE_NAME;}

Port_FirstStageVoltageFeedback& Port_FirstStageVoltageFeedback::initialize() noexcept{
	return SingletonBase<Port_FirstStageVoltageFeedback>::initialize(
		new FirstStageVoltageFeedbackFiberOpticDuplexChannel()
	);
}
Port_FirstStageVoltageFeedback::Port_FirstStageVoltageFeedback(TOSLINKDuplexChannel* toslinkDuplexChannel)
:Port_VoltageFeedbackBase(toslinkDuplexChannel){
}
