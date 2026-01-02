#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "Interfaces/IVoltageSource.hpp"
#include <memory>
#include <memory>
extern "C" {
    #include "freertos/FreeRTOS.h"
    #include "freertos/portmacro.h"
}
class ThresholdMonitor final:
	public IVoltageSource,
	public SingletonBase<ThresholdMonitor>{
    friend class SingletonBase<ThresholdMonitor>;
private:
	static inline constexpr const char* FLASH_NAMESPACE = "s";
	static inline constexpr const char* THRESHOLD_VOLTAGE_KEY = "tv";
	static const char* SET_TAP_VOLTAGE_THRESHOLD_ERROR_MESSAGE;
	const VoltageFeedbackModuleConfiguration& _config1;
	const VoltageFeedbackModuleConfiguration& _config2;
	std::shared_ptr<MonitorVoltageThresholdHandle> _monitorVoltageThresholdHandle;
	std::mutex _lock;
	std::optional<bool> _forceReached;
	bool _actualReached;
public:
	static const char* getTag();
	DISALLOW_COPY_MOVE(ThresholdMonitor);
	float getVoltage(uint16_t& raw) override;
	void setThresholdVoltage(float voltage);
	void setForce(std::optional<bool> value) noexcept;
protected:
    explicit ThresholdMonitor(
		adc_channel_t ch,
		const VoltageFeedbackModuleConfiguration& config1,
		const VoltageFeedbackModuleConfiguration& config2
	)noexcept;
private:
	void onVoltageThresholdReachedChanged(bool reached)noexcept;
	void toScaledADCThreshold(float vUnscaled, float& a, float& b) noexcept;
	float fromScaledADCToActual(float adc) noexcept;
};