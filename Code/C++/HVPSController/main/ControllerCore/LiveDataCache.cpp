#include "LiveDataCache.hpp"

LiveDataCache::LiveDataCache(Port_FirstStageVoltageFeedback& first,
                             Port_OutputVoltageFeedback& out) noexcept
: _port_firstStageVoltageFeedback(first)
, _port_OutputVoltageFeedback(out)
, _outputVoltage(0.0)
, _outputCurrent(0.0)
, _totalOutputEnergy(0.0)
, _firstStageVoltage(0.0)
, _peakPrimaryCurrent(0.0)
{
	_eventConnectionGotOutputVoltage = _port_OutputVoltageFeedback.onGotVoltage.addHandler(handleGotOutputVoltage);
	_eventConnectionGotFirstStageVoltage = _port_FirstStageVoltageFeedback.onGotVoltage.addHandler(handleGotFirstStageVoltage);
}

// Getters (relaxed: fast, fine for monotonic snapshots)
DoubleAndTime LiveDataCache::getOutputVoltage()const noexcept         { return _outputVoltage.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getOutputCurrent()const noexcept         { return _outputCurrent.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getTotalOutputEnergy()const noexcept     { return _totalOutputEnergy.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getFirstStageVoltage()const noexcept     { return _firstStageVoltage.load(std::memory_order_relaxed); }
DoubleAndTime LiveDataCache::getPeakPrimaryCurrent()const noexcept    { return _peakPrimaryCurrent.load(std::memory_order_relaxed); }

// Setters
void LiveDataCache::setOutputVoltage(double v) noexcept        { 
	_outputVoltage.store(DoubleAndTime(voltage, TimeHelper::us()), std::memory_order_relaxed); 
}
void LiveDataCache::setOutputCurrent(double v)  noexcept       { 
	_outputCurrent.store(DoubleAndTime(voltage, TimeHelper::us()), std::memory_order_relaxed);
	}
void LiveDataCache::setTotalOutputEnergy(double v) noexcept    { 
	_totalOutputEnergy.store(DoubleAndTime(voltage, TimeHelper::us()), std::memory_order_relaxed); 
}
void LiveDataCache::setFirstStageVoltage(double v)  noexcept   {
	_firstStageVoltage.store(DoubleAndTime(voltage, TimeHelper::us()), std::memory_order_relaxed); 
}
void LiveDataCache::setPeakPrimaryCurrent(double v) noexcept   {
	_peakPrimaryCurrent.store(DoubleAndTime(voltage, TimeHelper::us()), std::memory_order_relaxed); 
}

void LiveDataCache::handleGotOutputVoltage(double voltage) noexcept{
	setOutputVoltage(voltage);
}
void LiveDataCache::handleGotFirstStageVoltage(double voltage) noexcept{
	setFirstStageVoltage(voltage);
}
