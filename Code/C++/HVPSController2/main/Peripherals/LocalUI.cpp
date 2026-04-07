#include "LocalUI.hpp"
#include "Macros/GetFileName.hpp"
const char* LocalUI::getTag() {return GET_FILE_NAME;}
LocalUI::LocalUI(
	HighSpeedCore& highSpeedCore,
	HVPSLEDDisplay& hVPSLEDDisplay
) noexcept:
_highSpeedCore(highSpeedCore),
_hVPSLEDDisplay(hVPSLEDDisplay){
	_eventConnectionOnSystemStateChanged = highSpeedCore
		.onSystemStateChanged.addHandler([&](SystemState s){
		handleSystemStateChanged(s);
	});
}
void LocalUI::handleSystemStateChanged(SystemState systemState){
	LOG_INFO("LocalUI::handleSystemStateChanged");
	_hVPSLEDDisplay.indicateState(systemState);
	if(systemState==SystemState::Live){
		LOG_INFO("Was live");
	}
}