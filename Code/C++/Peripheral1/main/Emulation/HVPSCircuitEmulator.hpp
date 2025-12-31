#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/Peripheral1Config.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "IO/Inputs.hpp"
#include "Timing/FrequencyMeter.hpp"
class HVPSCircuitEmulator:public SingletonBase<HVPSCircuitEmulator>{
    friend class SingletonBase<HVPSCircuitEmulator>;
public:
	static const char* getTag();
	static inline constexpr float CONTINUOUS_OUTPUT_POWER_WATTS = 10.0f;
private:
	const HVPSConfiguration& _hvpsConfig;
	const VoltageFeedbackModuleConfiguration& _firstStageVoltageFeedbackModuleConfig;
	const VoltageFeedbackModuleConfiguration& _outputVoltageFeedbackModuleConfig;
	float _a, _b;
	float _currentVillardEnergyJouls;
	FrequencyMeter _frequencyMeter;
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
	void run();
	bool mosfetIsOn();
	void villardEnergyChanged(
		float currentVillardEnergyJouls);
};