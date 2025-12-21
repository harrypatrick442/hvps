#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "Interfaces/IGetCurrentVoltage.hpp"
#include <memory>
class ThresholdMonitor final:
	public SingletonBase<ThresholdMonitor>{
    friend class SingletonBase<ThresholdMonitor>;
private:
	static inline constexpr const char* FLASH_NAMESPACE = "s";
	static inline constexpr const char* THRESHOLD_VOLTAGE_KEY = "tv";
public:
	static const char* getTag();
	DISALLOW_COPY_MOVE(ThresholdMonitor);
	float getVoltage();
	void setThresholdVoltage(float voltage);
protected:
    explicit ThresholdMonitor(
		adc_channel_t ch,
		const VoltageFeedbackModuleConfiguration& config1,
		const VoltageFeedbackModuleConfiguration& config2
	)noexcept;
private:
	const VoltageFeedbackModuleConfiguration& _config1;
	const VoltageFeedbackModuleConfiguration& _config2;
	std::shared_ptr<MonitorVoltageThresholdHandle> _monitorVoltageThresholdHandle;
	
	void onVoltageThresholdReachedChanged(bool reached)noexcept;
	float toScaledADCThreshold(float vUnscaled)noexcept;
};