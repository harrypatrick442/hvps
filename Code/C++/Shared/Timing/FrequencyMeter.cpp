#include "Timing/FrequencyMeter.hpp"
#include "Logging/Log.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
FrequencyMeter::FrequencyMeter():
_cyclesRanCountPlusOne(1),
_startTimeForFrequencyMeasurement_ms(0),
_overflowed(false)
{
	
}
void FrequencyMeter::restart(){
	_cyclesRanCountPlusOne = 1;
	_startTimeForFrequencyMeasurement_ms = TimeHelper::ms();
	_overflowed = false;
}
void FrequencyMeter::tick(){
	_cyclesRanCountPlusOne++;
	if(_cyclesRanCountPlusOne==0){
		_overflowed = true;
	}
}
bool FrequencyMeter::calculateAndRestart(uint64_t& frequencyHz){
	uint64_t now_ms	= TimeHelper::ms();
	if(_overflowed||(_cyclesRanCountPlusOne==0)){
		_cyclesRanCountPlusOne = 1;
		_startTimeForFrequencyMeasurement_ms = now_ms;
		_overflowed = false;
		frequencyHz = 0;
		return false;
	}
	uint32_t nCycles = _cyclesRanCountPlusOne-1;
	uint64_t startTime_ms = _startTimeForFrequencyMeasurement_ms;
	_cyclesRanCountPlusOne = 1;
	_startTimeForFrequencyMeasurement_ms = now_ms;
	uint64_t dTms = now_ms - startTime_ms;
	if(dTms==0){
		LOG_INFO("Failed b");
		frequencyHz = 0;
		return false;
	}
	frequencyHz = static_cast<uint64_t>(nCycles*1000)/dTms;
	return true;
}
void FrequencyMeter::startPrintToConsoleLoop(){
	TaskFactory::createNonPriorityTask([this](){
		printToConsoleLoop();
	}, "FrequencyInfo");
}
void FrequencyMeter::printToConsoleLoop(){
	while(true){
		Delay::ms(1000);
		printToConsole();
	}
}
void FrequencyMeter::printToConsole(){
	uint64_t frequencyHz = 0;
	bool success = calculateAndRestart(frequencyHz);
	if(success){
		LOG_INFO("Frequency: %llu Hz", static_cast<unsigned long long>(frequencyHz));
	}
	else{
		LOG_INFO("Failed to calculate frequency");
	}
}