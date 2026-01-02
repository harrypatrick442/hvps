#include "LiveDataCache.hpp"
#include "Timing/TimeHelper.hpp"
#include "Macros/GetFileName.hpp"
const char* LiveDataCache::getTag() {return GET_FILE_NAME;}
LiveDataCache::LiveDataCache(Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
                             Port_OutputVoltageFeedback& portOutputVoltageFeedback) noexcept
: _portFirstStageVoltageFeedback(portFirstStageVoltageFeedback)
, _portOutputVoltageFeedback(portOutputVoltageFeedback)
{
	_eventConnectionGotOutputVoltage = _portOutputVoltageFeedback.onGotVoltage.addHandler(
		[this](VoltageWithRaw voltageWithRaw){
			this->setOutputVoltage(voltageWithRaw);;
		}
	);
	_eventConnectionGotFirstStageVoltage = 
		_portFirstStageVoltageFeedback
		.onGotVoltage
		.addHandler(
			[this](VoltageWithRaw voltageWithRaw){
				this->setFirstStageVoltage(voltageWithRaw);
			}
		);
}

// Getters (relaxed: fast, fine for monotonic snapshots)
VoltageWithRawAndTime LiveDataCache::getOutputVoltage()const noexcept         
{ return _outputVoltage.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getOutputCurrent()const noexcept         
{ return _outputCurrent.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getTotalOutputEnergy()const noexcept    
 { return _totalOutputEnergy.load(std::memory_order_relaxed); }
VoltageWithRawAndTime LiveDataCache::getFirstStageVoltage()const noexcept     
{ return _firstStageVoltage.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getPeakPrimaryCurrent()const noexcept    
{ return _peakPrimaryCurrent.load(std::memory_order_relaxed); }

// Setters
void LiveDataCache::setOutputVoltage(VoltageWithRaw voltageWithRaw) noexcept        { 
	_outputVoltage.store(
		VoltageWithRawAndTime(voltageWithRaw.voltage, voltageWithRaw.raw, TimeHelper::us()),
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setOutputCurrent(float voltage)  noexcept       { 
	_outputCurrent.store(
		FloatAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	);
}
void LiveDataCache::setTotalOutputEnergy(float voltage) noexcept    { 
	_totalOutputEnergy.store(
		FloatAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setFirstStageVoltage(VoltageWithRaw voltageWithRaw)  noexcept   {
	_firstStageVoltage.store(
		VoltageWithRawAndTime(voltageWithRaw.voltage, voltageWithRaw.raw, TimeHelper::us()), 
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setPeakPrimaryCurrent(float voltage) noexcept   {
	_peakPrimaryCurrent.store(
		FloatAndTime(voltage, TimeHelper::us()),
		std::memory_order_relaxed
	);
}
