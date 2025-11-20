#include "VoltageFeedbackModuleBase.hpp"
#include "Generated/FirstStageVoltageFeedbackModuleConfig.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "System/SubsystemIdentifier.hpp"
#include "Timing/Delay.hpp"
static const char *TAG = "FirstStageVoltageFeedbackModule";

extern "C" void app_main(void)
{
	Delay::ms(3000);
	SubsystemIdentifier::set(SubsystemIdentifiers::FirstStageVoltageFeedbackModule);
	VoltageFeedbackModuleBase::main(Config1, Config2);
}
