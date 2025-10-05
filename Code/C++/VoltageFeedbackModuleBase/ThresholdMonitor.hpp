#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Event.hpp"
#include "Core/Macros.hpp"
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include <memory>
class ThresholdMonitor final:
	public SingletonBase<ThresholdMonitor>{
    friend class SingletonBase<ThresholdMonitor>;
public:
	static inline constexpr const char* TAG = "ThresholdMonitor";
	DISALLOW_COPY_MOVE(ThresholdMonitor);
protected:
    explicit ThresholdMonitor(const Configuration& config1, const Configuration& config2)noexcept;
private:
	const Configuration& _config1;
	const Configuration& _config2;
	std::shared_ptr<MonitorVoltageThresholdHandle> _monitorVoltageThresholdHandle;
	EventConnection _eventConnectionVoltageThresholdChanged;
	
	void onVoltageThresholdReachedChanged(bool reached)noexcept;
	double toScaledADCThreshold(double vUnscaled)noexcept;
};