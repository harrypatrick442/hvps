#pragma once
#include <stdint.h>               // For uint32_t
class CPUClockFrequencyHelper{
public:
	static uint32_t getClockFrequencyHzExact();
	static uint32_t getClockFrequencyHzApproximate();
	static float getClockFrequencyMHZExact();
	static float getClockFrequencyMHZApproximate();
};