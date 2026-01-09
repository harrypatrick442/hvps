// ============================================================================
//  AnalogueInputs (ESP‑IDF ≥ v5.0)  –  **ADC Continuous‑Mode update**
//  Replaces the former I²S/SAR trick with the new adc_continuous driver.
//  One‑sample DMA frames → ≈5 µs conversion + ≈4–5 µs driver latency @100–200 kS/s.
// ============================================================================

/* =========================== AnalogueInputs.hpp ============================ */
#pragma once

#include "esp_adc/adc_continuous.h"   // NEW – IDF 5.x continuous driver
#include "esp_adc_cal.h"
#include <optional>
#include <memory>
#include <mutex>
#include <atomic>
#include "ReverseVoltageToRawLookup.hpp"
#include "MonitorVoltageThresholdHandle.hpp"
#include "IMonitorCurrentAndPowerHandle.hpp"
#include "MonitorCurrentAndPowerHandle.hpp"
#include "ADCSession.hpp"
#include "IADCSession.hpp"

// ------------------ configuration constants (change as needed) -------------
#define DEFAULT_VREF                          1100        // mV (fallback)
#define ADC_CHANNEL_VOLTAGE_DIVIDER_INPUT     ADC_CHANNEL_5  // GPIO 33 on ESP32
#define ADC_ATTEN_CFG                         ADC_ATTEN_DB_12
#define ADC_BIT_WIDTH_CFG                     ADC_BITWIDTH_12
#define KNOWN_EXTERNAL_REFERENCE_VOLTAGE      2.5         // volts
#define ADC_CHARACTERIZATION_VREF             1100        // mV for esp_adc_cal_characterize
#define SAMPLE_RATE_HZ                        200000      // 100 kHz – raise to 200 kHz if you like
// In continuous mode we specify frame‑size in **bytes**. 4 bytes == 1 sample.
#define CONT_FRAME_BYTES                      SOC_ADC_DIGI_RESULT_BYTES
#define MAX_STORE_BUF_BYTES                   16         /* internal ring (driver copies out fast).
 Kept small for quick read to clear since we dont need long samples just latest values*/

class ADC : public IADCSession{
private:
    static const char* TAG;
    static bool _initialized;
	static ADC* _instance;
    static std::atomic<bool> _inUse;
	static ReverseVoltageToRawLookup* _reverseLookup;
    // --- ADC continuous driver handle
    static adc_continuous_handle_t _adc_hdl;
	static std::mutex _mutexSetChannel;

    // --- calibration & bookkeeping
    static esp_adc_cal_characteristics_t* _adc_chars;
    static float _correctionFactor;      // user tweak multiplier
    static float _voltageToRaw;          // helper – volts → raw

    // channel we are currently sampling on (for simple single-channel use-case)
    static std::optional<adc_channel_t> _currentChannel;

	static void errorCheck(esp_err_t err);

public:
    // life-cycle
    static void initialize();
	static void use(adc_channel_t ch, const std::function<void(IADCSession&&)>& fn);
    // raw helpers
    uint16_t singleRawLatestSampleSelectedChannel();
    uint16_t averagedRawSampleSelectedChannel(int nSamples = 32);

    float   singleCorrectedVoltageSampleSelectedChannel();
    float   averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32);

    static float   convertRawToVoltage(uint16_t raw);
    static uint16_t convertVoltageToApproximateRaw(float voltage);

    void setChannel(adc_channel_t ch);
    float   getCorrection();
    float   getVoltage();
	bool getRawQuickly(uint16_t& value);
	void measureNReadsPerSecond();
	static std::shared_ptr<MonitorVoltageThresholdHandle> 
		monitorVoltageThresholdWithNewPriorityTask(
			adc_channel_t channel,
			float initialVoltage, 
			std::function<void(bool)> callback
	);
	std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
		float senseResistanceOhms, 
		float outputCurrentLimitingResistanceOhms,
		float cumulativeEnergyThresholdJ,
		float energyDisipatedJPerS,
		std::function<void(bool)> callback
	);
	static float getMinimumVoltageCanRead();
private:
    static void start();
    static void stop();
	static void _monitorVoltageThreshold(
		std::shared_ptr<MonitorVoltageThresholdHandle> handle);
	static void _monitorCurrentAndPower(
		std::shared_ptr<MonitorCurrentAndPowerHandle> handle
	);
};
