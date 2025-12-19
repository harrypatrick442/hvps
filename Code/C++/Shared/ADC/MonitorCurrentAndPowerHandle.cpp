#include "MonitorCurrentAndPowerHandle.hpp"

MonitorCurrentAndPowerHandle::MonitorCurrentAndPowerHandle(
    float senseResistanceOhms,
    float outputCurrentLimitingResistanceOhms,
    float cumulativeEnergyThresholdJ,
    float energyDisipatedJPerS,
    std::function<void(bool)> callback
) noexcept
    : _exit(false),
      _currentA(0),
      _energyDisipatedJPerUs(energyDisipatedJPerS * 1000.0),
      _senseResistanceOhms(senseResistanceOhms),
      _outputCurrentLimitingResistanceOhms(outputCurrentLimitingResistanceOhms),
      _cumulativeEnergyThresholdJ(cumulativeEnergyThresholdJ),
      callbackReachedThreshold(std::move(callback))
{
}

MonitorCurrentAndPowerHandle::~MonitorCurrentAndPowerHandle() noexcept {
    stop();
}

void MonitorCurrentAndPowerHandle::stop() noexcept {
    _exit.store(true, std::memory_order_relaxed);
}

bool MonitorCurrentAndPowerHandle::getExitFlag() const noexcept {
    return _exit.load(std::memory_order_relaxed);
}

void MonitorCurrentAndPowerHandle::setCurrentA(float value) noexcept {
    _currentA.store(value, std::memory_order_relaxed);
}

float MonitorCurrentAndPowerHandle::getCurrentA() const noexcept {
    return _currentA.load(std::memory_order_relaxed);
}

void MonitorCurrentAndPowerHandle::setEnergyDisipatedJPerS(float value) noexcept {
    _energyDisipatedJPerUs.store(value * 1000.0, std::memory_order_relaxed);
}

float MonitorCurrentAndPowerHandle::getEnergyDisipatedUjPerUs() const noexcept {
    return _energyDisipatedJPerUs.load(std::memory_order_relaxed);
}

float MonitorCurrentAndPowerHandle::getSenseResistanceOhms() const noexcept {
    return _senseResistanceOhms;
}

float MonitorCurrentAndPowerHandle::getOutputCurrentLimitingResistanceOhms() const noexcept {
    return _outputCurrentLimitingResistanceOhms;
}

float MonitorCurrentAndPowerHandle::getCumulativeEnergyThresholdJ() const noexcept {
    return _cumulativeEnergyThresholdJ;
}
