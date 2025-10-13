#ifndef MONITOR_CURRENT_AND_POWER_HANDLE_HPP
#define MONITOR_CURRENT_AND_POWER_HANDLE_HPP

#include "IMonitorCurrentAndPowerHandle.hpp"
#include <atomic>
#include <functional>

class MonitorCurrentAndPowerHandle : public IMonitorCurrentAndPowerHandle {
    std::atomic<bool> _exit;
    std::atomic<double> _currentA;
    std::atomic<double> _energyDisipatedJPerUs;

    double _senseResistanceOhms;
    double _outputCurrentLimitingResistanceOhms;
    double _cumulativeEnergyThresholdJ;

public:
    std::function<void(bool)> callbackReachedThreshold;

    MonitorCurrentAndPowerHandle(
        double senseResistanceOhms,
        double outputCurrentLimitingResistanceOhms,
        double cumulativeEnergyThresholdJ,
        double energyDisipatedJPerS,
        std::function<void(bool)> callbackReachedThreshold
    ) noexcept;

    ~MonitorCurrentAndPowerHandle() noexcept override;

    void stop() noexcept override;

    double getCurrentA() const noexcept override;
    void setCurrentA(double value) noexcept;

    double getSenseResistanceOhms() const noexcept;
    double getOutputCurrentLimitingResistanceOhms() const noexcept;
    double getCumulativeEnergyThresholdJ() const noexcept;
    void setEnergyDisipatedJPerS(double value) noexcept override;
    double getEnergyDisipatedUjPerUs() const noexcept;
    bool getExitFlag() const noexcept;
};

#endif
