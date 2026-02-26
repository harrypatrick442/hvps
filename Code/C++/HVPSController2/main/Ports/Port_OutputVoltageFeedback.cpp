#include "Port_OutputVoltageFeedback.hpp"
#include "../IO/OutputVoltageFeedbackFiberOpticDuplexChannel.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Macros/GetFileName.hpp"
const char* Port_OutputVoltageFeedback::getTag() {return GET_FILE_NAME;}

Port_OutputVoltageFeedback& Port_OutputVoltageFeedback::initialize()noexcept {
	return SingletonBase<Port_OutputVoltageFeedback>::initialize(
		new OutputVoltageFeedbackFiberOpticDuplexChannel()
	);
}
Port_OutputVoltageFeedback::Port_OutputVoltageFeedback(TOSLINKDuplexChannel* toslinkDuplexChannel)
:Port_VoltageFeedbackBase(toslinkDuplexChannel){
}
