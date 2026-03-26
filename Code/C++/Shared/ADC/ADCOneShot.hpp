#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc_cal.h"
#include <optional>
#include <memory>
#include <mutex>
#include <atomic>
#include "IMonitorCurrentAndPowerHandle.hpp"
#include "ADCSession.hpp"
#include "IADCSession.hpp"

#define ONESHOT_ATTEN_CFG                     ADC_ATTEN_DB_12
#define ONESHOT_BIT_WIDTH_CFG                 ADC_BITWIDTH_12
#define ONESHOT_KNOWN_EXTERNAL_REFERENCE_VOLTAGE  2.5f
#define ONESHOT_ADC_CHARACTERIZATION_VREF     1100

class ADCOneShot : public IADCSession {
private:
    static const char* TAG;
    static bool _initialized;
    static ADCOneShot* _instance;
    static std::atomic<bool> _inUse;
    static adc_oneshot_unit_handle_t _adc_hdl;
    static std::mutex _mutexSetChannel;

    // calibration
    static esp_adc_cal_characteristics_t* _adc_chars;
    static float _correctionFactor;

    // current channel
    static std::optional<adc_channel_t> _currentChannel;

    static void errorCheck(esp_err_t err);

public:
    // lifecycle
    static void initialize();
    static void use(adc_channel_t ch, const std::function<void(IADCSession&&)>& fn);

    // IADCSession interface - supported
    float getVoltage() override;
    bool getRawQuickly(uint16_t& value) override;
    void setChannel(adc_channel_t ch) override;
    float getCorrection() override;

    // voltage helpers
    static float convertRawToVoltage(uint16_t raw);
    static float getMinimumVoltageCanRead();

    // IADCSession interface - NOT supported in oneshot
    uint16_t singleRawLatestSampleSelectedChannel() override;
    uint16_t averagedRawSampleSelectedChannel(int nSamples = 32) override;
    float singleCorrectedVoltageSampleSelectedChannel() override;
    float averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32) override;
    void measureNReadsPerSecond() override;
    std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
        float senseResistanceOhms,
        float outputCurrentLimitingResistanceOhms,
        float cumulativeEnergyThresholdJ,
        float energyDisipatedJPerS,
        std::function<void(bool)> callback
    ) override;
};