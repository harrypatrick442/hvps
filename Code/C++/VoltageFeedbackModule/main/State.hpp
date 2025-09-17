#ifndef State_hpp
#define State_hpp
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "cJSON/cJSON.h"
class State{
    public :
		static const char* TAG;
		static const char* FLASH_NAMESPACE;
		static const char* THRESHOLD_VOLTAGE;
        static State& initialize(
			MonitorVoltageThresholdHandle* monitorVoltageThresholdHandle);
		void setVoltageThreshold(double value);
    private:
        static State* _instance;
		MonitorVoltageThresholdHandle* _monitorVoltageThresholdHandle;
        State(MonitorVoltageThresholdHandle* monitorVoltageThresholdHandle);
};

#endif // State_hpp