#pragma once
#include <memory>
#include <functional>
#include <utility>
#include "IADCSession.hpp"
#include "System/SafeAbort.hpp"

class ADCOneShotSession : public IADCSession {
public:
    explicit ADCOneShotSession(IADCSession* real) : _real(real) {}

    IADCSession* operator->() {
        if (!_real) SAFE_ABORT("Attempt to access ADCOneShot after scope expired");
        return _real;
    }
    void invalidate() noexcept { _real = nullptr; }

    ADCOneShotSession(const ADCOneShotSession&) = delete;
    ADCOneShotSession& operator=(const ADCOneShotSession&) = delete;
    ADCOneShotSession(ADCOneShotSession&& other) noexcept : _real(other._real) {
        other._real = nullptr;
    }
    ADCOneShotSession& operator=(ADCOneShotSession&& other) noexcept {
        if (this != &other) { _real = other._real; other._real = nullptr; }
        return *this;
    }

    float getVoltage() override { check(); return _real->getVoltage(); }
    bool getRawQuickly(uint16_t& value) override { check(); return _real->getRawQuickly(value); }

    // --------------- Not supported in oneshot ---------------
    uint16_t singleRawLatestSampleSelectedChannel() override { notSupported(); return 0; }
    uint16_t averagedRawSampleSelectedChannel(int) override { notSupported(); return 0; }
    float singleCorrectedVoltageSampleSelectedChannel() override { notSupported(); return 0; }
    float averagedCorrectedVoltageSampleSelectedChannel(int) override { notSupported(); return 0; }
    void setChannel(adc_channel_t) override { notSupported(); }
    float getCorrection() override { notSupported(); return 0; }
    void measureNReadsPerSecond() override { notSupported(); }
    std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
        float, float, float, float, std::function<void(bool)>
    ) override { notSupported(); return nullptr; }

private:
    IADCSession* _real = nullptr;
    void check() const {
        if (!_real) SAFE_ABORT("Attempt to use ADCOneShot after scope expired");
    }
    void notSupported() const {
        SAFE_ABORT("Method not supported in ADCOneShotSession");
    }
};