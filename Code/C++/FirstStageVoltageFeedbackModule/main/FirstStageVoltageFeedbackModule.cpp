#include "VoltageFeedbackModuleBase.hpp"
#include "Generated/FirstStageVoltageFeedbackModuleConfig.hpp"
static const char *TAG = "FirstStageVoltageFeedbackModule";

extern "C" void app_main(void)
{
	VoltageFeedbackModuleBase::main(Config1, Config2);
}
