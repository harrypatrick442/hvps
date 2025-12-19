#include "SoftStartHandler.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Core/ArrayHelper.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"

float SoftStartHandler::doSoftStart(const HVPSConfiguration& config1, const HVPSConfiguration& config2){
	Outputs::setSoftStartResistorBypassOnOff(false);
	float voltages[WINDOW_SAMPLES];
	size_t index = 0;
	float lastAverageVoltage=0.0f;
	uint64_t startTimeMs = TimeHelper::ms();
	uint64_t talkNext = startTimeMs +TALK_INTERVAL_MILLISECONDS;
	bool reachedMinimumVoltage = false;
	bool voltageStoppedIncreasing = false;
	float result = -1.0f;
	Inputs::useADCPowerSupplyVoltageFeedbackChannel([&](IADCSession&& adc){
		float minimumVoltageCanRead = ADC::getMinimumVoltageCanRead();
		Log::Info(TAG, "Minimum voltage can read was: %f", minimumVoltageCanRead);
		while(true){
			Delay::ms(SAMPLE_INTERVAL_MS);
			if(config1.vPsOverVadcRatio!=config2.vPsOverVadcRatio){
				Aborter::safeAbort(TAG, "A value for the potential divider ratio corrupted");
				return;
			}
			voltages[index++] = adc.getVoltage()*config1.vPsOverVadcRatio;
			if(index<WINDOW_SAMPLES){
				continue;
			}
			index=0;
			float averageVoltage = ArrayHelper::average(voltages, WINDOW_SAMPLES);
			Log::Info(TAG, "averageVoltage %f", averageVoltage);
			float dVoltage = averageVoltage - lastAverageVoltage;
			reachedMinimumVoltage = averageVoltage>=MINIMUM_VOLTAGE_TO_REACH;
			voltageStoppedIncreasing = dVoltage <=0.0f;
			if(reachedMinimumVoltage&&voltageStoppedIncreasing){
				Log::Info(TAG, "Reached minimum voltage and voltage stopped increasing", lastAverageVoltage);
				Outputs::setSoftStartResistorBypassOnOff(true);
				Delay::ms(RELAY_SWITCH_TIME_MILLISECONDS);
				result = averageVoltage;
				return;
			}
			lastAverageVoltage = averageVoltage;
			uint64_t now = TimeHelper::ms();
			if(now>talkNext){
				Log::Info(TAG, tellWhatWaitingOn(reachedMinimumVoltage, voltageStoppedIncreasing));
				talkNext = now+TALK_INTERVAL_MILLISECONDS;
			}
		}
	});
	return result;
}
const char* SoftStartHandler::tellWhatWaitingOn(bool reachedMinimumVoltage, bool voltageStoppedIncreasing) {
	if (reachedMinimumVoltage) {
		if (voltageStoppedIncreasing)
			return "Prohibited state";//Not reachable
		return "Waiting for voltage to stabilize...";
	}
	if (voltageStoppedIncreasing)
		return "Voltage has not reached minimum but has stopped increasing...";
	return "Waiting for voltage to reach minimum threshold and stop increasing...";
}