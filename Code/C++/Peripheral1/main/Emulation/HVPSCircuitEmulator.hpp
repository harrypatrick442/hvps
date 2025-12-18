#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/Peripheral1Config.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
class HVPSCircuitEmulator:public SingletonBase<HVPSCircuitEmulator>{
    friend class SingletonBase<HVPSCircuitEmulator>;
public:
	static inline constexpr const char* TAG = "HVPSCircuitEmulator";
private:
	HVPSConfiguration& _hvpsConfiguration;
	VoltageFeedbackModuleConfiguration& _firstStageVoltageFeedbackModuleConfig;
	VoltageFeedbackModuleConfiguration& _outputVoltageFeedbackModuleConfig;
public:
	DISALLOW_COPY_MOVE(HVPSCircuitEmulator);
protected:
	explicit HVPSCircuitEmulator(
		const HVPSConfiguration& hvpsConfig, 
		const VoltageFeedbackModuleConfiguration& firstStageVoltageFeedbackModuleConfig, 
		const VoltageFeedbackModuleConfiguration& outputVoltageFeedbackModuleConfig)noexcept;
	~HVPSCircuitEmulator();
private:
};