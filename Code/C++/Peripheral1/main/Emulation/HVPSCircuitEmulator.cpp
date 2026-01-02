#pragma once
#include "HVPSCircuitEmulator.hpp"
#include "IO/PinDefinitions.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Timing/CPUClockFrequencyHelper.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Tasks/TaskFactory.hpp"
#include "DAC/DAC.hpp"
#include <cmath>
#include "Macros/GetFileName.hpp"
#include "IO/IOInteruptHelper.hpp"
float HVPSCircuitEmulator::CONTINUOUS_OUTPUT_POWER_WATTS = 10.0f;
uint32_t HVPSCircuitEmulator::INTERVAL_CALCULATE_UPDATE_MS = 100;
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
	_mosfetCurrentlyOn(false),
	_mosfetWentOnAtCycles(0),
	_nOnCyclesToProcess(0),
	_startTime_us(TimeHelper::us()),
	_frequencyMeter(),
	_cpuClockFrequencyMHZ(CPUClockFrequencyHelper::getClockFrequencyMHZApproximate()),
	_outputVoltageVolts(0),
	_firstStageVoltageVolts(0)
{
	esp_err_t err =  IOInteruptHelper::setupPinEdgeInterupt(
		PinDefinitions::DRIVE_SIGNAL,
		driveSignalISRTrampoline,
		(void*)this,
		true,	//risingEdg
		true,	//fallingEdge
		false,	//pullUpEnabled
		false	//pullDownEnabled
	);
	
	TaskFactory::createPriorityTask([this](){
		loop();
	}, "HVPSCircuitEmulator::loop");
	/*TaskFactory::createNonPriorityTask([this](){
		printVoltagesLoop();
	}, "HVPSCircuitEmulator::printVoltagesLoop");*/
	float totalVillardCapacitanceFarads = _hvpsConfig.nVillardStages * 2.0f * _hvpsConfig.villardCapacitorCapacitanceFarads;
	_a = 2.0f/totalVillardCapacitanceFarads;
	//_frequencyMeter.startPrintToConsoleLoop();
	
}
HVPSCircuitEmulator::~HVPSCircuitEmulator(){
}
void HVPSCircuitEmulator::loop(){
		uint32_t oldNCycles = esp_cpu_get_cycle_count();
	while(true){
		uint32_t nOnCycles = _nOnCyclesToProcess.exchange(0, std::memory_order_relaxed);
		float onTimeUs = static_cast<float>(nOnCycles) / _cpuClockFrequencyMHZ;
		float energyIntoFlyback = _hvpsConfig.maxFlybackEnergyPerCycleJouls *
				powf(onTimeUs/ _hvpsConfig.onTimeMicroSeconds, 2.0f);
				
		uint32_t nowNCycles = esp_cpu_get_cycle_count();
		uint32_t dNCycles = nowNCycles - oldNCycles;
		oldNCycles = nowNCycles;
		float timeSinceLastLoopUs = static_cast<float>(dNCycles) / _cpuClockFrequencyMHZ;
		float energyOut = _b * timeSinceLastLoopUs;
		float newVillardEnergyJouls = _currentVillardEnergyJouls + energyIntoFlyback - energyOut;
		if(newVillardEnergyJouls<0)newVillardEnergyJouls = 0;
		villardEnergyChanged(newVillardEnergyJouls);
		_frequencyMeter.tick();
	}
}
void HVPSCircuitEmulator::printVoltagesLoop(){
		uint32_t oldNCycles = esp_cpu_get_cycle_count();
	while(true){
		LOG_INFO("Ouput voltage %f", _outputVoltageVolts);
		LOG_INFO("First stage voltage %f", _firstStageVoltageVolts);
		Delay::ms(100);
	}
}
void HVPSCircuitEmulator::villardEnergyChanged(
	float newVillardEnergyJouls)
{
	_currentVillardEnergyJouls = newVillardEnergyJouls;
	_outputVoltageVolts = std::sqrtf(_currentVillardEnergyJouls*_a);
	//LOG_INFO("output voltage is: %f", _outputVoltageVolts);
	_firstStageVoltageVolts = _outputVoltageVolts/_hvpsConfig.nVillardStages;
	//LOG_INFO("first stage voltage is: %f", _firstStageVoltageVolts);
	float firstStageVoltageFeedbackModuleTapVoltage = _firstStageVoltageVolts / _firstStageVoltageFeedbackModuleConfig.vHvOverVadcRatio;
	
	//LOG_INFO("Setting first stage voltage tap to : %f", firstStageVoltageFeedbackModuleTapVoltage);
	Outputs::setFirstStageVoltageFeedbackModuleTapVoltage(
	firstStageVoltageFeedbackModuleTapVoltage
	);
	float outputVoltageFeedbackModuleTapVoltage = _outputVoltageVolts / _outputVoltageFeedbackModuleConfig.vHvOverVadcRatio;
	//LOG_INFO("Setting output voltage tap to: %f", outputVoltageFeedbackModuleTapVoltage);

	Outputs::setOutputVoltageFeedbackModuleTapVoltage(
		outputVoltageFeedbackModuleTapVoltage
	);
}
void IRAM_ATTR HVPSCircuitEmulator::handleDriveSignalChanged(){
	bool mosfetIsOn = !Inputs::getDriveSignal();
	if(mosfetIsOn){
		if(_mosfetCurrentlyOn){
			return;
		}
		_mosfetCurrentlyOn = true;
		_mosfetWentOnAtCycles = esp_cpu_get_cycle_count();
		return;
	}
	if(!_mosfetCurrentlyOn){
		return;
	}
	_mosfetCurrentlyOn = false;
	if(_mosfetWentOnAtCycles==0){
		return;
	}
	uint32_t nOnCycles = esp_cpu_get_cycle_count() - _mosfetWentOnAtCycles;
	_nOnCyclesToProcess.fetch_add(nOnCycles, std::memory_order_relaxed); 
}
void IRAM_ATTR HVPSCircuitEmulator::driveSignalISRTrampoline(void* arg){
    static_cast<HVPSCircuitEmulator*>(arg)->handleDriveSignalChanged();
}