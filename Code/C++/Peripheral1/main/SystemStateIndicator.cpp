#include "SystemStateIndicator.hpp"
#include "Macros/GetFileName.hpp"
const char* SystemStateIndicator::getTag() {return GET_FILE_NAME;}
SystemStateIndicator::SystemStateIndicator(HVPSLEDDisplay& hVPSLEDDisplay):
_hVPSLEDDisplay(hVPSLEDDisplay){
	
}
void SystemStateIndicator::indicateState(SystemState systemState)
{
	_hVPSLEDDisplay.indicateState(systemState);
}