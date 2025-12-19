#ifndef MONITOR_CURRENT_AND_POWER_HANDLE_HPP
#define MONITOR_CURRENT_AND_POWER_HANDLE_HPP

#include "IMonitorCurrentAndPowerHandle.hpp"
#include <atomic>
#include <functional>

class MonitorCurrentAndPowerHandle : public IMonitorCurrentAndPowerHandle {
    std::atomic<bool> _exit;
    std::atomic<float> _currentA;
    std::atomic<float> _energyDisipatedJPerUs;

    float _senseResistanceOhms;
    float _outputCurrentLimitingResistanceOhms;
    float _cumulativeEnergyThresholdJ;

public:
    std::function<void(bool)> callbackReachedThreshold;

    MonitorCurrentAndPowerHandle(
        float senseResistanceOhms,
        float outputCurrentLimitingResistanceOhms,
        float cumulativeEnergyThresholdJ,
        float energyDisipatedJPerS,
        std::function<void(bool)> callbackReachedThreshold
    ) noexcept;

    ~MonitorCurrentAndPowerHandle() noexcept override;

    void stop() noexcept override;

    float getCurrentA() const noexcept override;
    void setCurrentA(float value) noexcept;

    float getSenseResistanceOhms() const noexcept;
    float getOutputCurrentLimitingResistanceOhms() const noexcept;
    float getCumulativeEnergyThresholdJ() const noexcept;
    void setEnergyDisipatedJPerS(float value) noexcept override;
    float getEnergyDisipatedUjPerUs() const noexcept;
    bool getExitFlag() const noexcept;
};

#endif
