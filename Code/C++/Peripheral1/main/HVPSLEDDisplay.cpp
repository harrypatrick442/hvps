#pragma once
#include "HVPSLEDDisplay.hpp"
#include "IO/PinDefinitions.hpp"
size_t HVPSLEDDisplay::LED_STRIP_LENGTH = 4;
uint32_t HVPSLEDDisplay::OFF_COLOUR = 0;
HVPSLEDDisplay::HVPSLEDDisplay(const Configuration& config):
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
			staticColour(_config.idleColour);
			break;
		case SystemState::Live:
			flashColour(_config.liveColour);
			break;
		case SystemState::RunningSystemChecks:
			flashColour(_config.runningSystemChecksColour);
			break;
		case SystemState::ShuttingDown:
			flashColour(_config.shuttingDownColour);
			break;
		case SystemState::ShutDown:
			staticColour(_config.shutDownColour);
			break;
		case SystemState::Error:
			flashColour(_config.errorColour);
			break;
		case SystemState::Unknown:
		default:
			staticColour(_config.unknownColour);
			break;
	}
}
void HVPSLEDDisplay::staticColour(uint32_t colour){
	_timerFlash->stop();
	std::unique_lock<std::mutex> lock(_mutex);
	_flashing = false;
	_currentColour = colour;
	displayColour(_currentColour);
	
}
void HVPSLEDDisplay::flashColour(uint32_t colour){
	std::unique_lock<std::mutex> lock(_mutex);
	_flashing = true;
	_flashingIsOn = true;
	_currentColour = colour;
	displayColour(_currentColour);
	lock.unlock();
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
}