#pragma once
#include "HVPSCircuitEmulator.hpp"
#include "IO/PinDefinitions.hpp"
#include <cmath>
HVPSCircuitEmulator::HVPSCircuitEmulator(const HVPSConfig& hvpsConfig):
	_hvpsConfig(hvpsConfig),
	_firstStageVoltageFeedbackModuleConfig(firstStageVoltageFeedbackModuleConfig),
	_outputVoltageFeedbackModuleConfig(outputVoltageFeedbackModuleConfig)
{
	double totalVillardCapacitanceFarads = _hvpsConfig.nVillardStages * 2d * _hvpsConfig.villardCapacitorCapacitanceFarads;
	_a = 2d/totalVillardCapacitanceFarads;
}
HVPSCircuitEmulator::~HVPSCircuitEmulator(){
	TaskFactory::createPriorityTask(
		[this](){
			run();
		},
		"HVPSCircuitEmulator");
}
void HVPSCircuitEmulator::run(){
	double currentVillardEnergyJouls = 0;
	uint64_t now = TimeHelper::us();
	while(true){
		uint64_t turnOnTimeUs = now;
		while(mosfetIsOn()){
			
		}
		now = TimeHelper::us();
		uint64_t turnOffTimeUs = now;
		double timePrimaryWasOnUs = static_cast<uint64_t>(turnOffTimeUs - turnOnTimeUs);
		
		//peak current is proportional to time on.
		//Energy is proportional to peak current squared
		//Energy is therefore proportional to time on.
		double energyIntoFlyback;
		if(timePrimaryWasOnUs>_hvpsConfig.onTimeMicroSeconds){
			timePrimaryWasOnUs = _hvpsConfig.onTimeMicroSeconds;
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls;
		}
		else if{timePrimaryWasOnUs<_hvpsConfig.onTimeMicroSeconds){
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls * std::pow(timePrimaryWasOnUs / _hvpsConfig.onTimeMicroSeconds, 2d);
		}
		else{
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls;
		}
		double energyOutOfVillardWhileMosefetOn = continuousOutputPowerWatts * timePrimaryWasOnUs/1000000d;
		double newVillardEnergyJouls = currentVillardEnergyJouls + energyIntoFlyback - energyOutOfVillardWhileMosefetOn;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
		while(!mosfetIsOn()){
			
		}
		now = TimeHelper::us();
		double offTimeUs = static_cast<double>(now - turnOffTimeUs);
		double energyOutOfVillardWhileMosfetOff = continuousOutputPowerWatts * offTimeUs;
		
		newVillardEnergyJouls = currentVillardEnergyJouls - energyOutOfVillardWhileMosfetOff;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
	}
}
void HVPSCircuitEmulator::villardEnergyChanged(double currentVillardEnergyJouls, double firstStageVoltageToTapVoltage, double outputVoltageToTapVoltage){
	double outputVoltageVolts = std::sqrt(currentVillardEnergyJouls*a);
	double firstStageVoltageVolts = outputVoltageVolts/_hvpsConfig.nVillardStages;
	_dac.setChannel1Voltage(outputVoltageVolts / _outputVoltageFeedbackModuleConfig.vHvOverVadcRatio);
	_dac.setChannel2Voltage(firstStageVoltageVolts / _firstStageVoltageFeedbackModuleConfig.vHvOverVadcRatio);
}