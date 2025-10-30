#include "LiveDataCache.hpp"
#include "Timing/TimeHelper.hpp"
LiveDataCache::LiveDataCache(Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
                             Port_OutputVoltageFeedback& portOutputVoltageFeedback) noexcept
: _portFirstStageVoltageFeedback(portFirstStageVoltageFeedback)
, _portOutputVoltageFeedback(portOutputVoltageFeedback)
{
	_eventConnectionGotOutputVoltage = _portOutputVoltageFeedback.onGotVoltage.addHandler(
		[this](double voltage){
			this->handleGotOutputVoltage(voltage);
		}
	);
	_eventConnectionGotFirstStageVoltage = 
		_portFirstStageVoltageFeedback
		.onGotVoltage
		.addHandler(
			[this](double voltage){
				handleGotFirstStageVoltage(voltage);
			}
		);
}

// Getters (relaxed: fast, fine for monotonic snapshots)
DoubleAndTime LiveDataCache::getOutputVoltage()const noexcept         
{ return _outputVoltage.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getOutputCurrent()const noexcept         
{ return _outputCurrent.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getTotalOutputEnergy()const noexcept    
 { return _totalOutputEnergy.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getFirstStageVoltage()const noexcept     
{ return _firstStageVoltage.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getPeakPrimaryCurrent()const noexcept    
{ return _peakPrimaryCurrent.load(std::memory_order_relaxed); }

// Setters
void LiveDataCache::setOutputVoltage(double voltage) noexcept        { 
	_outputVoltage.store(
		DoubleAndTime(voltage, TimeHelper::us()),
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setOutputCurrent(double voltage)  noexcept       { 
	_outputCurrent.store(
		DoubleAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	);
}
void LiveDataCache::setTotalOutputEnergy(double voltage) noexcept    { 
	_totalOutputEnergy.store(
		DoubleAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setFirstStageVoltage(double voltage)  noexcept   {
	_firstStageVoltage.store(
		DoubleAndTime(voltage, TimeHelper::us()), 
		std::memory_order_relaxed
	); 
}
void LiveDataCache::setPeakPrimaryCurrent(double voltage) noexcept   {
	_peakPrimaryCurrent.store(
		DoubleAndTime(voltage, TimeHelper::us()),
		std::memory_order_relaxed
	);
}

void LiveDataCache::handleGotOutputVoltage(double voltage) noexcept{
	setOutputVoltage(voltage);
}
void LiveDataCache::handleGotFirstStageVoltage(double voltage) noexcept{
	setFirstStageVoltage(voltage);
}
