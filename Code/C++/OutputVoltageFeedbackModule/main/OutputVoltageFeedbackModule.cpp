#include "VoltageFeedbackModuleBase.hpp"
#include "Generated/OutputVoltageFeedbackModuleConfig.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "System/SubsystemIdentifier.hpp"
#include "Timing/Delay.hpp"
static const char *TAG = "OutputVoltageFeedbackModule";

extern "C" void app_main(void)
{
	Delay::ms(3000);
	SubsystemIdentifier::set(SubsystemIdentifiers::OutputVoltageFeedbackModule);
	VoltageFeedbackModuleBase::main(Config1, Config2);
}
