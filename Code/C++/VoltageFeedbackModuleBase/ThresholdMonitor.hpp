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
	static inline constexpr const char* TAG = "ThresholdMonitor";
	DISALLOW_COPY_MOVE(ThresholdMonitor);
	double getVoltage();
	void setThresholdVoltage(double voltage);
protected:
    explicit ThresholdMonitor(
		adc_channel_t ch,
		const Configuration& config1,
		const Configuration& config2
	)noexcept;
private:
	const Configuration& _config1;
	const Configuration& _config2;
	std::shared_ptr<MonitorVoltageThresholdHandle> _monitorVoltageThresholdHandle;
	
	void onVoltageThresholdReachedChanged(bool reached)noexcept;
	double toScaledADCThreshold(double vUnscaled)noexcept;
};