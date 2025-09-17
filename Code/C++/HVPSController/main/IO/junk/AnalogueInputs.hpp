// ============================================================================
//  AnalogueInputs (ESP‑IDF ≥ v5.0)  –  **ADC Continuous‑Mode update**
//  Replaces the former I²S/SAR trick with the new adc_continuous driver.
//  One‑sample DMA frames → ≈5 µs conversion + ≈4–5 µs driver latency @100–200 kS/s.
// ----------------------------------------------------------------------------
//  This single file shows BOTH header (.hpp) and implementation (.cpp).
//  Copy / split as you prefer.
// ============================================================================

/* =========================== AnalogueInputs.hpp ============================ */
#ifndef ANALOGUE_INPUTS_HPP
#define ANALOGUE_INPUTS_HPP

#include "esp_adc/adc_continuous.h"   // NEW – IDF 5.x continuous driver
#include "esp_adc_cal.h"
#include <optional>
#include "../ADC/ReverseVoltageToRawLookup.hpp"
#include "../ADC/MonitorVoltageThresholdHandle.hpp"

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

class AnalogueInputs {
private:
    static const char* TAG;
    static bool _initialized;
	static ReverseVoltageToRawLookup* _reverseLookup;
    // --- ADC continuous driver handle
    static adc_continuous_handle_t _adc_hdl;

    // --- calibration & bookkeeping
    static esp_adc_cal_characteristics_t* _adc_chars;
    static double _correctionFactor;      // user tweak multiplier
    static double _voltageToRaw;          // helper – volts → raw

    // channel we are currently sampling on (for simple single-channel use-case)
    static std::optional<adc_channel_t> _currentChannel;

    static void setChannel(adc_channel_t ch);
	static void errorCheck(esp_err_t err);

public:
    // life-cycle
    static void initialize();

    // raw helpers
    static uint16_t singleRawLatestSampleSelectedChannel();
    static uint16_t averagedRawSampleSelectedChannel(int nSamples = 32);

    // voltage helpers
    static double   singleCorrectedVoltageSampleSelectedChannel();
    static double   averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32);

    static double   convertRawToVoltage(uint16_t raw);
    static uint16_t convertVoltageToApproximateRaw(double voltage);

    // user convenience
    static void     selectVoltageDividerInputAsChannel();

    static double   getCorrection();
    static double   getVoltage();
	static MonitorVoltageThresholdHandle* monitorVoltageThresholdWithNewPriorityTask(double initialVoltage);
	static void measureNReadsPerSecond();
	private:
		static void _monitorVoltageThreshold(void* pointer);
};
#endif
