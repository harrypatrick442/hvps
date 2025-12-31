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
	const VoltageFeedbackModuleConfiguration& _config1;
	const VoltageFeedbackModuleConfiguration& _config2;
	std::shared_ptr<MonitorVoltageThresholdHandle> _monitorVoltageThresholdHandle;
	std::atomic<bool> _forceReached;
	std::atomic<bool> _actualReached;
public:
	static const char* getTag();
	DISALLOW_COPY_MOVE(ThresholdMonitor);
	float getVoltage();
	void setThresholdVoltage(float voltage);
	void setForce(bool value) noexcept;
protected:
    explicit ThresholdMonitor(
		adc_channel_t ch,
		const VoltageFeedbackModuleConfiguration& config1,
		const VoltageFeedbackModuleConfiguration& config2
	)noexcept;
private:
	void onVoltageThresholdReachedChanged(bool reached)noexcept;
	float toScaledADCThreshold(float vUnscaled)noexcept;
};