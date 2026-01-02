#pragma once
#include <cstdint>
class FrequencyMeter{
private:
	volatile uint32_t _cyclesRanCountPlusOne;
	volatile uint64_t _startTimeForFrequencyMeasurement_ms;
	volatile bool _overflowed;
public:
	FrequencyMeter();
	void restart();
	void tick();
	bool calculateAndRestart(uint64_t& frequencyHz);
	void startPrintToConsoleLoop();
	void printToConsole();
private:
	void printToConsoleLoop();
};