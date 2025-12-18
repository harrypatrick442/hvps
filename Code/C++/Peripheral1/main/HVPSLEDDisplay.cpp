#pragma once
#include "HVPSLEDDisplay.hpp"
#include "IO/PinDefinitions.hpp"
size_t HVPSLEDDisplay::LED_STRIP_LENGTH = 4;
uint32_t HVPSLEDDisplay::OFF_COLOUR = 0;
HVPSLEDDisplay::HVPSLEDDisplay(const Peripheral1Configuration& config):
_config(config),
_currentColour(0),
_flashing(false),
_flashingIsOn(false)
{
	_ledStrip = new LEDStrip(
			PinDefinitions::LEDS_D_OUT, LED_STRIP_LENGTH);
	_displayBuffer = new DisplayBuffer(_ledStrip->getLength());
	_ledStrip->initialize(_displayBuffer);
	_pixels = new volatile uint32_t*[LED_STRIP_LENGTH];
	_ledStrip->getPixels(LED_STRIP_LENGTH, _pixels);
	_timerFlash = new Timer(500, [this](){
		timerCallback();
	}, true);
	staticColour(config.unknownColour);
	
}
HVPSLEDDisplay::~HVPSLEDDisplay(){
	delete _ledStrip;
	_ledStrip = nullptr;
	delete _displayBuffer;
	_displayBuffer = nullptr;
	delete _pixels;
	_pixels = nullptr;
}
void HVPSLEDDisplay::indicateState(SystemState systemState){
	switch(systemState){
		case SystemState::Idle:
			//Log::Info(TAG, "Idle");
			show(_config.idleColour, _config.idleFlashDelayMs);
			break;
		case SystemState::Live:
			//Log::Info(TAG, "Live");
			show(_config.liveColour, _config.liveFlashDelayMs);
			break;
		case SystemState::RunningSystemChecks:
			//Log::Info(TAG, "RunningSystemChecks");
			show(_config.runningSystemChecksColour, _config.runningSystemChecksFlashDelayMs);
			break;
		case SystemState::ShuttingDown:
			//Log::Info(TAG, "ShuttingDown");
			show(_config.shuttingDownColour, _config.shuttingDownFlashDelayMs);
			break;
		case SystemState::ShutDown:
			//Log::Info(TAG, "ShutDown");
			show(_config.shutDownColour, _config.shutDownFlashDelayMs);
			break;
		case SystemState::Error:
			//Log::Info(TAG, "Error");
			show(_config.errorColour, _config.errorFlashDelayMs);
			break;
		case SystemState::Unknown:
			//Log::Info(TAG, "Unknown");
		default:
			show(_config.unknownColour, _config.unknownFlashDelayMs);
			break;
	}
}
void HVPSLEDDisplay::show(uint32_t colour, uint32_t flashDelayMilliseconds){
	if(flashDelayMilliseconds>0){
		flashColour(colour, flashDelayMilliseconds);
		return;
	}
	staticColour(colour);
}
void HVPSLEDDisplay::staticColour(uint32_t colour){
	_timerFlash->stop();
	std::unique_lock<std::mutex> lock(_mutex);
	_flashing = false;
	_currentColour = colour;
	displayColour(_currentColour);
	
}
void HVPSLEDDisplay::flashColour(uint32_t colour, uint32_t flashDelayMilliseconds){
	std::unique_lock<std::mutex> lock(_mutex);
	_flashing = true;
	_flashingIsOn = true;
	_currentColour = colour;
	displayColour(_currentColour);
	lock.unlock();
	_timerFlash->stop();
	_timerFlash->setIntervalMs(flashDelayMilliseconds);
	_timerFlash->start();
}
void HVPSLEDDisplay::timerCallback(){
	std::unique_lock<std::mutex> lock(_mutex);
	if(!_flashing){
		return;
	}
	if(_flashingIsOn){
		_flashingIsOn = false;
		displayColour(OFF_COLOUR);
		return;
	}
	_flashingIsOn = true;
	displayColour(_currentColour);
}
void HVPSLEDDisplay::displayColour(uint32_t colour){
	for(size_t i=0; i<LED_STRIP_LENGTH; i++){
		*(*(_pixels+i)) = colour;
	}
	_ledStrip->refresh();
}