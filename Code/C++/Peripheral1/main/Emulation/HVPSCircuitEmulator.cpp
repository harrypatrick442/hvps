#pragma once
#include "HVPSCircuitEmulator.hpp"
#include "IO/PinDefinitions.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Timing/TimeHelper.hpp"
#include <mathf>
HVPSCircuitEmulator::HVPSCircuitEmulator(
	const HVPSConfiguration& hvpsConfig, 
	const VoltageFeedbackModuleConfiguration& firstStageVoltageFeedbackModuleConfig, 
	const VoltageFeedbackModuleConfiguration& outputVoltageFeedbackModuleConfig):
	_hvpsConfig(hvpsConfig),
	_firstStageVoltageFeedbackModuleConfig(firstStageVoltageFeedbackModuleConfig),
	_outputVoltageFeedbackModuleConfig(outputVoltageFeedbackModuleConfig),
{
	float totalVillardCapacitanceFarads = _hvpsConfig.nVillardStages * 2d * _hvpsConfig.villardCapacitorCapacitanceFarads;
	_a = 2d/totalVillardCapacitanceFarads;
}
HVPSCircuitEmulator::~HVPSCircuitEmulator(){
	if(!TaskFactory::createPriorityTask(
		[this](){
			run();
		},
		"HVPSCircuitEmulator")
	){
			Aborter::safeAbort(TAG, "Failed to start loop");
	}
}
void HVPSCircuitEmulator::run(){
	float currentVillardEnergyJouls = 0;
	uint64_t now = TimeHelper::us();
	while(true){
		uint64_t turnOnTimeUs = now;
		while(mosfetIsOn()){
			
		}
		now = TimeHelper::us();
		uint64_t turnOffTimeUs = now;
		uint64_t timePrimaryWasOnUs = static_cast<uint64_t>(turnOffTimeUs - turnOnTimeUs);
		
		//peak current is proportional to time on.
		//Energy is proportional to peak current squared
		//Energy is therefore proportional to time on.
		float energyIntoFlyback;
		if(timePrimaryWasOnUs>_hvpsConfig.onTimeMicroSeconds){
			timePrimaryWasOnUs = _hvpsConfig.onTimeMicroSeconds;
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls;
		}
		else if(timePrimaryWasOnUs<_hvpsConfig.onTimeMicroSeconds){
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls * std::pow(static_cast<float>(timePrimaryWasOnUs)/ _hvpsConfig.onTimeMicroSeconds, 2d);
		}
		else{
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls;
		}
		float energyOutOfVillardWhileMosefetOn = continuousOutputPowerWatts * static_cast<float>(timePrimaryWasOnUs)/1000000f;
		float newVillardEnergyJouls = currentVillardEnergyJouls + energyIntoFlyback - energyOutOfVillardWhileMosefetOn;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
		while(!mosfetIsOn()){
			
		}
		now = TimeHelper::us();
		float offTimeUs = static_cast<float>(now - turnOffTimeUs);
		float energyOutOfVillardWhileMosfetOff = continuousOutputPowerWatts * offTimeUs;
		
		newVillardEnergyJouls = currentVillardEnergyJouls - energyOutOfVillardWhileMosfetOff;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
	}
}
void HVPSCircuitEmulator::villardEnergyChanged(
	float currentVillardEnergyJouls, 
	float firstStageVoltageToTapVoltage, 
	float outputVoltageToTapVoltage)
{
	float outputVoltageVolts = std::sqrtf(currentVillardEnergyJouls*a);
	float firstStageVoltageVolts = outputVoltageVolts/_hvpsConfig.nVillardStages;
	Outputs::setFirstStageVoltageFeedbackModuleTapVoltage(
		firstStageVoltageVolts / _firstStageVoltageFeedbackModuleConfig.vHvOverVadcRatio);
	Outputs::setOutputVoltageFeedbackModuleTapVoltage(
		outputVoltageVolts / _outputVoltageFeedbackModuleConfig.vHvOverVadcRatio);
}
bool HVPSCircuitEmulator::mosfetIsOn(){
	return ! Inputs::getDriveSignal();
}