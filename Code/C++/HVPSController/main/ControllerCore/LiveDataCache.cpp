#include "LiveDataCache.hpp"
#include "Timing/TimeHelper.hpp"
LiveDataCache::LiveDataCache(Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
                             Port_OutputVoltageFeedback& portOutputVoltageFeedback) noexcept
: _portFirstStageVoltageFeedback(portFirstStageVoltageFeedback)
, _portOutputVoltageFeedback(portOutputVoltageFeedback)
{
	_eventConnectionGotOutputVoltage = _portOutputVoltageFeedback.onGotVoltage.addHandler(
		[this](float voltage){
			this->handleGotOutputVoltage(voltage);
		}
	);
	_eventConnectionGotFirstStageVoltage = 
		_portFirstStageVoltageFeedback
		.onGotVoltage
		.addHandler(
			[this](float voltage){
				handleGotFirstStageVoltage(voltage);
			}
		);
}

// Getters (relaxed: fast, fine for monotonic snapshots)
FloatAndTime LiveDataCache::getOutputVoltage()const noexcept         
{ return _outputVoltage.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getOutputCurrent()const noexcept         
{ return _outputCurrent.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getTotalOutputEnergy()const noexcept    
 { return _totalOutputEnergy.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getFirstStageVoltage()const noexcept     
{ return _firstStageVoltage.load(std::memory_order_relaxed); }
FloatAndTime LiveDataCache::getPeakPrimaryCurrent()const noexcept    
{ return _peakPrimaryCurrent.load(std::memory_order_relaxed); }

// Setters
void LiveDataCache::setOutputVoltage(float voltage) noexcept        { 
	_outputVoltage.store(
		FloatAndTime(voltage, TimeHelper::us()),
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
void LiveDataCache::setFirstStageVoltage(float voltage)  noexcept   {
	_firstStageVoltage.store(
		FloatAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setPeakPrimaryCurrent(float voltage) noexcept   {
	_peakPrimaryCurrent.store(
		FloatAndTime(voltage, TimeHelper::us()),
		std::memory_order_relaxed
	);
}

void LiveDataCache::handleGotOutputVoltage(float voltage) noexcept{
	setOutputVoltage(voltage);
}
void LiveDataCache::handleGotFirstStageVoltage(float voltage) noexcept{
	setFirstStageVoltage(voltage);
}
