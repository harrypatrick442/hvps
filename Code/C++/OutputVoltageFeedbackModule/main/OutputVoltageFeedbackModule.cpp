#include "VoltageFeedbackModuleBase.hpp"
#include "Generated/OutputVoltageFeedbackModuleConfig.hpp"
static const char *TAG = "OutputVoltageFeedbackModule";

extern "C" void app_main(void)
{
	VoltageFeedbackModuleBase::main(Config1, Config2);
}
