#include "SoftStartHandler.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Core/ArrayHelper.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"

double SoftStartHandler::doSoftStart(const Configuration& config1, const Configuration& config2){
	Outputs::setSoftStartResistorBypassOnOff(false);
	Inputs::selectADCPowerSupplyVoltageFeedbackChannel();
	double voltages[WINDOW_SAMPLES];
	size_t index = 0;
	double lastAverageVoltage=0.0;
	double minimumVoltageCanRead = ADC::getMinimumVoltageCanRead();
	Log::Info(TAG, "Minimum voltage can read was: %f", minimumVoltageCanRead);
	uint64_t talkNext = TimeHelper::ms()+TALK_INTERVAL_MILLISECONDS;
	bool reachedMinimumVoltage = false;
	bool voltageStoppedIncreasing = false;
	while(true){
		Delay::ms(SAMPLE_INTERVAL_MS);
		if(config1.vPsOverVadcRatio!=config2.vPsOverVadcRatio){
			Aborter::safeAbort(TAG, "A value for the potential divider ratio corrupted");
			return -1;
		}
		voltages[index++] = Inputs::getADCVoltage()*config1.vPsOverVadcRatio;
		if(index<WINDOW_SAMPLES){
			continue;
		}
		index=0;
		double averageVoltage = ArrayHelper::average(voltages, WINDOW_SAMPLES);
		//Log::Info(TAG, "averageVoltage %f", averageVoltage);
		//Log::Info(TAG, "lastAverageVoltage %f", lastAverageVoltage);
		double dVoltage = averageVoltage - lastAverageVoltage;
		reachedMinimumVoltage = averageVoltage>=MINIMUM_VOLTAGE_TO_REACH;
		voltageStoppedIncreasing = dVoltage <=0.0;
		if(reachedMinimumVoltage&&voltageStoppedIncreasing){
			Outputs::setSoftStartResistorBypassOnOff(true);
			Delay::ms(RELAY_SWITCH_TIME_MILLISECONDS);
			return averageVoltage;
		}
		lastAverageVoltage = averageVoltage;
		uint64_t now = TimeHelper::ms();
		if(now>talkNext){
			Log::Info(TAG, tellWhatWaitingOn(reachedMinimumVoltage, voltageStoppedIncreasing));
			talkNext = now+TALK_INTERVAL_MILLISECONDS;
		}
	}
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