#ifndef I_MONITOR_CURRENT_AND_POWER_HANDLE_HPP
#define I_MONITOR_CURRENT_AND_POWER_HANDLE_HPP

class IMonitorCurrentAndPowerHandle {
public:
    virtual ~IMonitorCurrentAndPowerHandle() = default;

    virtual void stop() noexcept = 0;

    virtual float getCurrentA() const noexcept = 0;
    virtual void setEnergyDisipatedJPerS(float value) noexcept = 0;
};

#endif
