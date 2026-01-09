#pragma once
#include <memory>
#include <functional>
#include <utility>       // for std::move (used in callbacks)
#include "IADCSession.hpp"
#include "System/SafeAbort.hpp"

class ADCSession : public IADCSession {

public:
    explicit ADCSession(IADCSession* real) : _real(real) {}

    // ---------------- Operator & lifecycle ----------------
    IADCSession* operator->() {
        if (!_real) {
            SAFE_ABORT("Attempt to access ADC after scope expired");
        }
        return _real;
    }

    void invalidate() noexcept {
        _real = nullptr;
    }

    ADCSession(const ADCSession&) = delete;
    ADCSession& operator=(const ADCSession&) = delete;

    ADCSession(ADCSession&& other) noexcept : _real(other._real) {
        other._real = nullptr;
    }

    ADCSession& operator=(ADCSession&& other) noexcept {
        if (this != &other) {
            _real = other._real;
            other._real = nullptr;
        }
        return *this;
    }

    // ---------------- Interface Forwarding ----------------
    uint16_t singleRawLatestSampleSelectedChannel() override {
        check();
        return _real->singleRawLatestSampleSelectedChannel();
    }

    uint16_t averagedRawSampleSelectedChannel(int nSamples = 32) override {
        check();
        return _real->averagedRawSampleSelectedChannel(nSamples);
    }

    float singleCorrectedVoltageSampleSelectedChannel() override {
        check();
        return _real->singleCorrectedVoltageSampleSelectedChannel();
    }

    float averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32) override {
        check();
        return _real->averagedCorrectedVoltageSampleSelectedChannel(nSamples);
    }

    void setChannel(adc_channel_t ch) override {
        check();
        _real->setChannel(ch);
    }

    float getCorrection() override {
        check();
        return _real->getCorrection();
    }

    float getVoltage() override {
        check();
        return _real->getVoltage();
    }
	bool getRawQuickly(uint16_t& value) override{
        check();
        return _real->getRawQuickly(value);
    }

    void measureNReadsPerSecond() override {
        check();
        _real->measureNReadsPerSecond();
    }

    std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
        float senseResistanceOhms,
        float outputCurrentLimitingResistanceOhms,
        float cumulativeEnergyThresholdJ,
        float energyDisipatedJPerS,
        std::function<void(bool)> callback
    ) override {
        check();
        return _real->monitorCurrentAndPower(
            senseResistanceOhms,
            outputCurrentLimitingResistanceOhms,
            cumulativeEnergyThresholdJ,
            energyDisipatedJPerS,
            std::move(callback)
        );
    }

private:
    IADCSession* _real = nullptr;

    void check() const {
        if (!_real) {
            SAFE_ABORT("Attempt to use ADC after scope expired");
        }
    }
};
