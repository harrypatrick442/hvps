#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Event.hpp"
#include "Core/Macros.hpp"
#include "ADC/IMonitorCurrentAndPowerHandle.hpp"
#include "Generated/CurrentAndPowerFeedbackModuleConfiguration.hpp"
#include <memory>
class CurrentAndPowerThresholdMonitor final:
	public SingletonBase<CurrentAndPowerThresholdMonitor>{
    friend class SingletonBase<CurrentAndPowerThresholdMonitor>;
public:
	static const char* getTag();
	DISALLOW_COPY_MOVE(CurrentAndPowerThresholdMonitor);
protected:
    explicit CurrentAndPowerThresholdMonitor(const Configuration& config1, const Configuration& config2)noexcept;
private:
	const Configuration& _config1;
	const Configuration& _config2;
	std::shared_ptr<IMonitorCurrentAndPowerHandle> _monitorCurrentAndPowerHandle;
	
	void validateConfigs(Configuration& config1, Configuration& config2)noexcept;
	void onThresholdReachedChanged(bool reached)noexcept
};