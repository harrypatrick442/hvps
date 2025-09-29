#include "SoftStartHandler.hpp"
#include "../Core/Delay.hpp"
#include "AnalogueInputs.hpp"
#include "../Core/ArrayHelper.hpp"

double SoftStartHandler::doSoftStart(){
	Outputs::setSoftStartResistorBypassOnOff(false);
	AnalogueInputs::selectPowerSupplyVoltageFeedbackChannel();
	double voltages[WINDOW_SAMPLES];
	size_t index = 0;
	double lastAverageVoltage=0.0;
	while(true){
		Delay::ms(SAMPLE_INTERVAL_MS);
		voltages[index++] = AnalogueInputs::getVoltage();
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