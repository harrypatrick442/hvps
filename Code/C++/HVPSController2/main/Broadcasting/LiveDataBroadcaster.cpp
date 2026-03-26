#include "LiveDataBroadcaster.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Macros/GetFileName.hpp"
#include "Enums/ValueBoundType.hpp"
#include "Logging/Log.hpp"
const char* LiveDataBroadcaster::getTag() {return GET_FILE_NAME;}
LiveDataBroadcaster::LiveDataBroadcaster(
	Port_ControllingMachine& portControllingMachine,
	HighSpeedCore& highSpeedCore,
	TemperatureMonitor& temperatureMonitor,
	ITemperatureSensor& mosfetTemperatureSensor
):
	_portControllingMachine(portControllingMachine),
	_highSpeedCore(highSpeedCore),
	_temperatureMonitor(temperatureMonitor),
	_mosfetTemperatureSensor(mosfetTemperatureSensor),
	_timer(500, 
		[this](){
			this->_run();
		}, true
	){
	_eventConnectionPortOnOpen = _portControllingMachine.onOpened.addHandler(
		[this](){
			this->start();
		}
	);
	_eventConnectionPortOnClose = _portControllingMachine.onClosed.addHandler(
		[this](){
			this->stop();
		}
	);
	if(_portControllingMachine.getIsOpen()){
		this->start();
	}
}
void LiveDataBroadcaster::start(){
	_timer.start();
}
void LiveDataBroadcaster::stop(){
	_timer.stop();
}
void LiveDataBroadcaster::_run(){
	ValueBoundType frequencyHzValueBoundType;
	uint64_t frequencyHz = _highSpeedCore.getFrequencyHz(frequencyHzValueBoundType);
	float outputVoltage = _highSpeedCore.getActualOutputVoltage();
	float firstStageVoltage = _highSpeedCore.getActualFirstStageVoltage();
	ValueBoundType peakPrimaryCurrentValueBoundType;
	
	
	
	LiveDataMessage liveDataMessage(
		static_cast<uint8_t>(ValueBoundType::Approximate),
		firstStageVoltage,
		frequencyHz,
		static_cast<uint8_t>(frequencyHzValueBoundType),
		_temperatureMonitor.getTemperature(_mosfetTemperatureSensor),
		/*outputCurrent*/0.0f,//_highSpeedCore.getOutputCurrent().f, 
		static_cast<uint8_t>(ValueBoundType::Approximate),
		outputVoltage,
		_highSpeedCore.getActualPeakPrimaryCurrent(),
		static_cast<uint8_t>(ValueBoundType::Approximate),
		/*primaryPower*/0.0f,
		/*totalOutputEnergy*/0.0f,//_highSpeedCore.getTotalOutputEnergy().f,
		/*totalPrimaryEnergy*/0.0f);
	_portControllingMachine.sendLiveData(liveDataMessage);
}