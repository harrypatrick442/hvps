#pragma once
#include "HVPSCircuitEmulator.hpp"
#include "IO/PinDefinitions.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Timing/TimeHelper.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Timing/Delay.hpp"
#include "DAC/DAC.hpp"
#include <cmath>
#include "Macros/GetFileName.hpp"
const char* HVPSCircuitEmulator::getTag() {return GET_FILE_NAME;}
HVPSCircuitEmulator::HVPSCircuitEmulator(
	const HVPSConfiguration& hvpsConfig, 
	const VoltageFeedbackModuleConfiguration& firstStageVoltageFeedbackModuleConfig, 
	const VoltageFeedbackModuleConfiguration& outputVoltageFeedbackModuleConfig):
	_hvpsConfig(hvpsConfig),
	_firstStageVoltageFeedbackModuleConfig(firstStageVoltageFeedbackModuleConfig),
	_outputVoltageFeedbackModuleConfig(outputVoltageFeedbackModuleConfig),
	_b(CONTINUOUS_OUTPUT_POWER_WATTS/1000000.0f),
	_currentVillardEnergyJouls(0),
	_frequencyMeter()
{
	float totalVillardCapacitanceFarads = _hvpsConfig.nVillardStages * 2.0f * _hvpsConfig.villardCapacitorCapacitanceFarads;
	_a = 2.0f/totalVillardCapacitanceFarads;
	if(!TaskFactory::createPriorityTask(
		[this](){
			run();
		},
		"HVPSCircuitEmulator")
	){
		SAFE_ABORT("Failed to start loop");
	}
	_frequencyMeter.startPrintToConsoleLoop();
	
}
HVPSCircuitEmulator::~HVPSCircuitEmulator(){
}
void HVPSCircuitEmulator::run(){
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
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls *
				powf(static_cast<float>(timePrimaryWasOnUs)/ _hvpsConfig.onTimeMicroSeconds, 2.0f);
		}
		else{
			energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls;
		}
		float energyOutOfVillardWhileMosefetOn = _b * static_cast<float>(timePrimaryWasOnUs);
		float newVillardEnergyJouls = _currentVillardEnergyJouls + energyIntoFlyback - energyOutOfVillardWhileMosefetOn;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
		while(!mosfetIsOn()){
		}
		now = TimeHelper::us();
		float timePrimaryWasOffUs = static_cast<float>(now - turnOffTimeUs);
		float energyOutOfVillardWhileMosfetOff = _b * timePrimaryWasOffUs;
		
		newVillardEnergyJouls = _currentVillardEnergyJouls - energyOutOfVillardWhileMosfetOff;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
	}
}
void HVPSCircuitEmulator::villardEnergyChanged(
	float newVillardEnergyJouls)
{
	_currentVillardEnergyJouls = newVillardEnergyJouls;
	float outputVoltageVolts = std::sqrtf(_currentVillardEnergyJouls*_a);
	float firstStageVoltageVolts = outputVoltageVolts/_hvpsConfig.nVillardStages;
	Outputs::setFirstStageVoltageFeedbackModuleTapVoltage(
		firstStageVoltageVolts / _firstStageVoltageFeedbackModuleConfig.vHvOverVadcRatio
	);
	Outputs::setOutputVoltageFeedbackModuleTapVoltage(
		outputVoltageVolts / _outputVoltageFeedbackModuleConfig.vHvOverVadcRatio
	);
}
bool HVPSCircuitEmulator::mosfetIsOn(){
	return ! Inputs::getDriveSignal();
}