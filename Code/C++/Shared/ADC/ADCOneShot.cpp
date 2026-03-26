#include "ADCOneShot.hpp"
#include "esp_log.h"
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "../Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "../Core/CleanupBucket.hpp"
#include "Macros/GetFileName.hpp"

// ---- static member definitions ----
const char* ADCOneShot::TAG           = "ADCOneShot";
bool ADCOneShot::_initialized         = false;
ADCOneShot* ADCOneShot::_instance     = new ADCOneShot();
std::atomic<bool> ADCOneShot::_inUse(false);
adc_oneshot_unit_handle_t ADCOneShot::_adc_hdl = nullptr;
esp_adc_cal_characteristics_t* ADCOneShot::_adc_chars = nullptr;
float ADCOneShot::_correctionFactor   = 1.0f;
std::optional<adc_channel_t> ADCOneShot::_currentChannel = std::nullopt;
std::mutex ADCOneShot::_mutexSetChannel;

// ============================================================
//  Lifecycle
// ============================================================

void ADCOneShot::initialize() {
    if (_initialized) {
        LOG_WARN("ADCOneShot already initialized");
        return;
    }
    _initialized = true;

    // allocate calibration struct
    _adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    errorCheck(!_adc_chars ? ESP_ERR_NO_MEM : ESP_OK);

    // characterise ADC1
    esp_adc_cal_characterize(
        ADC_UNIT_1,
        ONESHOT_ATTEN_CFG,
        ADC_WIDTH_BIT_12,
        ONESHOT_ADC_CHARACTERIZATION_VREF,
        _adc_chars
    );

    // create oneshot unit
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id  = ADC_UNIT_1,
        .clk_src  = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    errorCheck(adc_oneshot_new_unit(&init_cfg, &_adc_hdl));
}

void ADCOneShot::setChannel(adc_channel_t ch) {
    std::unique_lock<std::mutex> lock(_mutexSetChannel);
    if (_currentChannel && _currentChannel.value() == ch) return; // already on it
	LOG_INFO("ADCOneShot::setChannel");
    adc_oneshot_chan_cfg_t config = {
		.atten = ADC_ATTEN_DB_11,
		.bitwidth = ADC_BITWIDTH_DEFAULT,
	};
	LOG_INFO("ADCOneShot::setChannel A");
    errorCheck(adc_oneshot_config_channel(_adc_hdl, ch, &config));
	LOG_INFO("ADCOneShot::setChannel B");
    _currentChannel = ch;
}

void ADCOneShot::use(adc_channel_t ch, const std::function<void(IADCSession&&)>& fn) {
    bool expected = false;
    if (!_inUse.compare_exchange_strong(expected, true)) {
        SAFE_ABORT("ADCOneShot::use() called while already in use");
        return;
    }
    CleanupBucket cleanup;
    cleanup.addCallback([&]() noexcept {
        _inUse.store(false);
    });
    _instance->setChannel(ch);
    ADCSession proxy(static_cast<IADCSession*>(_instance));
    fn(std::move(proxy));
    proxy.invalidate();
}

// ============================================================
//  Voltage / raw reads
// ============================================================

float ADCOneShot::getVoltage() {
    uint16_t raw = 0;
    if (!getRawQuickly(raw)) return 0.0f;
    return convertRawToVoltage(raw);
}

bool ADCOneShot::getRawQuickly(uint16_t& value) {
    if (!_currentChannel) return false;
    int raw = 0;
    esp_err_t err = adc_oneshot_read(_adc_hdl, _currentChannel.value(), &raw);
    if (err != ESP_OK) return false;
    value = static_cast<uint16_t>(raw);
    return true;
}

float ADCOneShot::convertRawToVoltage(uint16_t raw) {
    float mv = esp_adc_cal_raw_to_voltage(raw, _adc_chars);
    return (mv * _correctionFactor) / 1000.0f;
}

float ADCOneShot::getCorrection() {
    return _correctionFactor;
}

float ADCOneShot::getMinimumVoltageCanRead() {
    return convertRawToVoltage(0);
}

// ============================================================
//  Error checking
// ============================================================

void ADCOneShot::errorCheck(esp_err_t err) {
    if (err == ESP_OK) return;
    char msg[64];
    esp_err_to_name_r(err, msg, sizeof(msg));
    SAFE_ABORT("Fatal ADCOneShot error: %s", msg);
}

// ============================================================
//  Not supported in oneshot mode
// ============================================================

uint16_t ADCOneShot::singleRawLatestSampleSelectedChannel() {
    SAFE_ABORT("singleRawLatestSampleSelectedChannel not supported in ADCOneShot");
    return 0;
}
uint16_t ADCOneShot::averagedRawSampleSelectedChannel(int) {
    SAFE_ABORT("averagedRawSampleSelectedChannel not supported in ADCOneShot");
    return 0;
}
float ADCOneShot::singleCorrectedVoltageSampleSelectedChannel() {
    SAFE_ABORT("singleCorrectedVoltageSampleSelectedChannel not supported in ADCOneShot");
    return 0.0f;
}
float ADCOneShot::averagedCorrectedVoltageSampleSelectedChannel(int) {
    SAFE_ABORT("averagedCorrectedVoltageSampleSelectedChannel not supported in ADCOneShot");
    return 0.0f;
}
void ADCOneShot::measureNReadsPerSecond() {
    SAFE_ABORT("measureNReadsPerSecond not supported in ADCOneShot");
}
std::shared_ptr<IMonitorCurrentAndPowerHandle> ADCOneShot::monitorCurrentAndPower(
    float, float, float, float, std::function<void(bool)>
) {
    SAFE_ABORT("monitorCurrentAndPower not supported in ADCOneShot");
    return nullptr;
}