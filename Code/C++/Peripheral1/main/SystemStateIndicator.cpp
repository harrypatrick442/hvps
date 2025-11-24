#include "SystemStateIndicator.hpp"
SystemStateIndicator::SystemStateIndicator(HVPSLEDDisplay& hVPSLEDDisplay):
_hVPSLEDDisplay(hVPSLEDDisplay){
	
}
void SystemStateIndicator::indicateState(SystemState systemState)
{
	_hVPSLEDDisplay.indicateState(systemState);
}