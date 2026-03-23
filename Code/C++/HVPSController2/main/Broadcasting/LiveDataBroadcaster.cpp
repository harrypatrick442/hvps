#include "LiveDataBroadcaster.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Macros/GetFileName.hpp"
#include "Enums/ValueBoundType.hpp"
#include "Logging/Log.hpp"
const char* LiveDataBroadcaster::getTag() {return GET_FILE_NAME;}
LiveDataBroadcaster::LiveDataBroadcaster(
	LiveDataCache& liveDataCache, 
	Port_ControllingMachine& portControllingMachine,
	HighSpeedCore& highSpeedCore,
	TemperatureMonitor& temperatureMonitor,
	ITemperatureSensor& mosfetTemperatureSensor
):
	_liveDataCache(liveDataCache),
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
	VoltageWithRawAndTime outputVoltage = _liveDataCache.getOutputVoltage();
	VoltageWithRawAndTime firstStageVoltage = _liveDataCache.getFirstStageVoltage();
	ValueBoundType peakPrimaryCurrentValueBoundType;
	LiveDataMessage liveDataMessage(
		static_cast<uint8_t>(firstStageVoltage.raw == 0? ValueBoundType::MinimumKnown:ValueBoundType::Approximate),
		firstStageVoltage.voltage,
		frequencyHz,
		static_cast<uint8_t>(frequencyHzValueBoundType),
		_temperatureMonitor.getTemperature(_lowerSnubberDiodeTemperatureSensor),
		_temperatureMonitor.getTemperature(_mosfetTemperatureSensor),
		_liveDataCache.getOutputCurrent().f, 
		static_cast<uint8_t>(outputVoltage.raw == 0? ValueBoundType::MinimumKnown:ValueBoundType::Approximate),
		outputVoltage.voltage,
		_highSpeedCore.getPeakPrimaryCurrent(peakPrimaryCurrentValueBoundType),
		static_cast<uint8_t>(peakPrimaryCurrentValueBoundType),
		/*primaryPower*/0.0f,
		_liveDataCache.getTotalOutputEnergy().f,
		/*totalPrimaryEnergy*/0.0f);
	_portControllingMachine.sendLiveData(liveDataMessage);
}