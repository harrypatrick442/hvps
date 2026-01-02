#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/Peripheral1Config.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "IO/Inputs.hpp"
#include "Timing/Timer.hpp"
#include "Timing/FrequencyMeter.hpp"
class HVPSCircuitEmulator:public SingletonBase<HVPSCircuitEmulator>{
    friend class SingletonBase<HVPSCircuitEmulator>;
public:
	static float CONTINUOUS_OUTPUT_POWER_WATTS;
	static uint32_t INTERVAL_CALCULATE_UPDATE_MS;
	static const char* getTag();
private:
	const HVPSConfiguration& _hvpsConfig;
	const VoltageFeedbackModuleConfiguration& _firstStageVoltageFeedbackModuleConfig;
	const VoltageFeedbackModuleConfiguration& _outputVoltageFeedbackModuleConfig;
	float _a, _b;
	float _currentVillardEnergyJouls;
	bool _mosfetCurrentlyOn;
	uint32_t _mosfetWentOnAtCycles;
	std::atomic<uint32_t> _nOnCyclesToProcess;
	uint64_t _startTime_us;
	FrequencyMeter _frequencyMeter;
	float _cpuClockFrequencyMHZ;
	float _outputVoltageVolts;
	float _firstStageVoltageVolts;
public:
	DISALLOW_COPY_MOVE(HVPSCircuitEmulator);
protected:
	explicit HVPSCircuitEmulator(
		const HVPSConfiguration& hvpsConfig, 
		const VoltageFeedbackModuleConfiguration& firstStageVoltageFeedbackModuleConfig, 
		const VoltageFeedbackModuleConfiguration& outputVoltageFeedbackModuleConfig
	)noexcept;
	~HVPSCircuitEmulator();
private:
	void loop();
	void printVoltagesLoop();
	void villardEnergyChanged(float currentVillardEnergyJouls);
	void IRAM_ATTR handleDriveSignalChanged();
	static void IRAM_ATTR driveSignalISRTrampoline(void* arg);
};