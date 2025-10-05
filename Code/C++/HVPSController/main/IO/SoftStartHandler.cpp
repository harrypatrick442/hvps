#include "SoftStartHandler.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Core/ArrayHelper.hpp"
#include "System/Aborter.hpp"
#include "Timing/Delay.hpp"

double SoftStartHandler::doSoftStart(const Configuration& config1, const Configuration& config2){
	Outputs::setSoftStartResistorBypassOnOff(false);
	Inputs::selectADCPowerSupplyVoltageFeedbackChannel();
	double voltages[WINDOW_SAMPLES];
	size_t index = 0;
	double lastAverageVoltage=0.0;
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
		double dVoltage = averageVoltage - lastAverageVoltage;
		if((lastAverageVoltage > 0)&&(dVoltage <=0.0)){
			Outputs::setSoftStartResistorBypassOnOff(true);
			Delay::ms(RELAY_SWITCH_TIME_MILLISECONDS);
			return averageVoltage;
		}
		lastAverageVoltage = averageVoltage;
	}
}