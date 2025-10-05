#pragma once
#include "Core/Event.hpp"
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "cJSON/cJSON.h"
class NonVolatileState{
	static inline constexpr const char* TAG = "State";
	static inline constexpr const char* FLASH_NAMESPACE = "s";
	static inline constexpr const char* THRESHOLD_VOLTAGE = "tv";
    public : 
		static Event<double> onVoltageThresholdChanged;
		static bool getVoltageThreshold(double& value);
		static void setVoltageThreshold(double value);
};