
/* ========================== AnalogueInputs.cpp ============================ */
#include "AnalogueInputs.hpp"
#include "esp_log.h"
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "../Timing/Delay.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "Outputs.hpp"
#include "esp_timer.h"
#include "../Tasks/TaskFactory.hpp"


const char* AnalogueInputs::TAG = "AnalogueInputs";
bool        AnalogueInputs::_initialized    = false;
adc_continuous_handle_t AnalogueInputs::_adc_hdl = nullptr;
esp_adc_cal_characteristics_t* AnalogueInputs::_adc_chars = nullptr;
double AnalogueInputs::_correctionFactor = 1.0;
double AnalogueInputs::_voltageToRaw     = 0.0;
std::optional<adc_channel_t> AnalogueInputs::_currentChannel = std::nullopt;
ReverseVoltageToRawLookup* AnalogueInputs::_reverseLookup = nullptr;
void AnalogueInputs::initialize()
{
    if (_initialized) {
        Log::Info(TAG, "Already initialized"); // (tag, warning format...)
        return;
    }
    _initialized = true;

    // --- allocate calibration struct
    _adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    errorCheck(!_adc_chars ? ESP_ERR_NO_MEM : ESP_OK); // basic null check with ESP-style error handling

    // --- Characterise ADC
    esp_adc_cal_characterize(
        ADC_UNIT_1,                   // which ADC unit to characterize
        ADC_ATTEN_CFG,                // attenuation level used during sampling
        ADC_WIDTH_BIT_12,             // resolution setting (e.g. 12-bit)
        ADC_CHARACTERIZATION_VREF,   // known VREF (mV)
        _adc_chars                    // pointer to results struct
    );

    _voltageToRaw = ((1ULL << ADC_BIT_WIDTH_CFG) - 1) / (KNOWN_EXTERNAL_REFERENCE_VOLTAGE);
	_reverseLookup = new ReverseVoltageToRawLookup(50, _adc_chars);
}
void AnalogueInputs::setChannel(adc_channel_t ch)
{
    if (_currentChannel && _currentChannel.value() == ch) return;   // already on it

    // Stop the driver if running
    if (_adc_hdl) {
        errorCheck(adc_continuous_stop(_adc_hdl)); // (adc_continuous_handle_t handle)
    }

    // Configure one‑channel pattern
    adc_continuous_config_t dig_cfg = {
        .pattern_num    = 1,                             // number of patterns used (1 channel)
        .sample_freq_hz = SAMPLE_RATE_HZ,               // desired ADC sampling frequency in Hz
        .conv_mode      = ADC_CONV_SINGLE_UNIT_1,       // only ADC unit 1 used
        .format         = ADC_DIGI_OUTPUT_FORMAT_TYPE1, // output format with unit/channel/type1
    };
    static adc_digi_pattern_config_t pattern;           // static to persist memory
    pattern.unit      = ADC_UNIT_1;                     // use ADC unit 1
    pattern.channel   = ch;                             // desired ADC channel
    pattern.bit_width = ADC_BIT_WIDTH_CFG;              // ADC resolution setting
    pattern.atten     = ADC_ATTEN_CFG;                  // input attenuation setting
    dig_cfg.adc_pattern = &pattern;                     // pointer to pattern config

    // (re-)configure continuous driver
    if (_adc_hdl) {
        errorCheck(adc_continuous_config(
            _adc_hdl,   // adc_continuous_handle_t handle
            &dig_cfg    // const adc_continuous_config_t* config
        ));
    } else {
        adc_continuous_handle_cfg_t h_cfg = {
            .max_store_buf_size = MAX_STORE_BUF_BYTES,         // internal driver ring buffer size (bytes)
            .conv_frame_size    = SOC_ADC_DIGI_DATA_BYTES_PER_CONV,  // size of each read frame (must be multiple of sample size)
        };
        errorCheck(adc_continuous_new_handle(
            &h_cfg,     // const adc_continuous_handle_cfg_t* config
            &_adc_hdl   // adc_continuous_handle_t* out_handle
        ));
        errorCheck(adc_continuous_config(
            _adc_hdl,   // handle
            &dig_cfg    // pattern config
        ));
    }

    errorCheck(adc_continuous_start(_adc_hdl)); // (adc_continuous_handle_t handle)
    _currentChannel = ch;
}

// -------------------- RAW sample helpers ---------------------------------
uint16_t AnalogueInputs::singleRawLatestSampleSelectedChannel()
{
    adc_digi_output_data_t d;
    uint32_t len = 0;

    // drain backlog
    while (adc_continuous_read(
        _adc_hdl,                                // handle
        reinterpret_cast<uint8_t*>(&d),          // output buffer
        sizeof(d),                               // buffer size in bytes (must be multiple of sample size)
        &len,                                     // number of bytes actually read
        0                                        // timeout (0 = non-blocking)
    ) == ESP_OK && len == sizeof(d)) {
        // loop; last read wins
    }

    // blocking read → freshest sample (~5 µs old)
    while(true){
		esp_err_t err = adc_continuous_read(
			_adc_hdl,                                // handle
			reinterpret_cast<uint8_t*>(&d),          // buffer
			sizeof(d),                               // size
			&len,                                     // out: bytes read
			portMAX_DELAY                            // timeout (wait forever)
		);
		if(err!= ESP_OK){
			continue;
		}
		if(len!=4){
			continue;
		}
		return d.type1.data & 0x0FFF;
	}

}

uint16_t AnalogueInputs::averagedRawSampleSelectedChannel(int nSamples)
{
    if (nSamples <= 0) return 0;
    uint32_t sum = 0;
    adc_digi_output_data_t d;
    uint32_t len = 0;
    int count = 0;

    while (count < nSamples) {
        esp_err_t err = adc_continuous_read(
            _adc_hdl,                              // handle
            reinterpret_cast<uint8_t*>(&d),        // output buffer
            sizeof(d),                             // buffer size
            &len,                                   // actual bytes read
            portMAX_DELAY                          // timeout
        );
        if (err == ESP_OK && len == sizeof(d)) {
            uint16_t raw = d.type1.data & 0x0FFF;
            sum += raw;
            ++count;
        }
    }
    return static_cast<uint16_t>(sum / nSamples);
}


// -------------------- Voltage helpers ------------------------------------
double AnalogueInputs::convertRawToVoltage(uint16_t raw)
{
    double mv = esp_adc_cal_raw_to_voltage(
        raw,        // raw ADC reading
        _adc_chars  // pointer to previously characterized calibration data
    );
    return (mv * _correctionFactor) / 1000.0; // convert mV to V
}


uint16_t AnalogueInputs::convertVoltageToApproximateRaw(double voltage)
{
	return (uint16_t)_reverseLookup->lookupVolts(voltage);
}

// ---------------- public wrap‑ups ----------------------------------------
double AnalogueInputs::singleCorrectedVoltageSampleSelectedChannel()
{
    return convertRawToVoltage(singleRawLatestSampleSelectedChannel());
}

double AnalogueInputs::averagedCorrectedVoltageSampleSelectedChannel(int nSamples)
{
    return convertRawToVoltage(averagedRawSampleSelectedChannel(nSamples));
}

void AnalogueInputs::selectReferenceVoltageChannel()
{
    setChannel(ADCChannels::REFERENCE_VOLTAGE);
}
void AnalogueInputs::selectSnubberVoltageFeedbackChannel()
{
    setChannel(ADCChannels::SNUBBER_VOLTAGE_FEEDBACK);
}
void AnalogueInputs::selectPrimaryCurrentFeedbackChannel()
{
    setChannel(ADCChannels::PRIMARY_CURRENT_FEEDBACK);
}
void AnalogueInputs::selectPowerSupplyVoltageFeedbackChannel()
{
    setChannel(ADCChannels::POWER_SUPPLY_VOLTAGE_FEEDBACK);
}

double AnalogueInputs::getCorrection()
{
    return _correctionFactor;
}
MonitorVoltageThresholdHandle* AnalogueInputs::monitorVoltageThresholdWithNewPriorityTask(
		double initialVoltage, 
		std::function<void(bool)> callback
	) {
	MonitorVoltageThresholdHandle* handle = new MonitorVoltageThresholdHandle(
		initialVoltage, _reverseLookup,  std::move(callback));
	
	TaskFactory::createPriorityTask(
		_monitorVoltageThreshold,
		handle,
		"AnalogueInputs::monitorVoltageThreshold"
    );
	return handle;
}
double AnalogueInputs::getVoltage(){
	adc_digi_output_data_t d;
	uint32_t len = 0;
	esp_err_t err;
	while(true){
		err = adc_continuous_read(
			_adc_hdl,                                // handle
			reinterpret_cast<uint8_t*>(&d),          // buffer
			sizeof(d),                               // size
			&len,                                     // out: bytes read
			portMAX_DELAY                            // timeout (wait forever)
		);
		if(err!= ESP_OK){
			continue;
		}
		if(len!=4){
			continue;
		}
		return convertRawToVoltage(d.type1.data & 0x0FFF);
	}
}
void AnalogueInputs::_monitorVoltageThreshold(void* pointer) {
	MonitorVoltageThresholdHandle* handle = static_cast<MonitorVoltageThresholdHandle*>(pointer);
	
	adc_digi_output_data_t d; // Stores a single read value
	uint32_t len = 0;
	esp_err_t err;
	bool set = false;
	bool isOn = false;
	while(!handle->exit.load(std::memory_order_relaxed)){
		err = adc_continuous_read(
			_adc_hdl,                              // handle
			reinterpret_cast<uint8_t*>(&d),        // buffer
			4,                             // size
			&len,                                   // out bytes
			0                                       // non-blocking
		); 
		if(err!= ESP_OK){
			continue;
		}
		if(len!=4){
			continue;
		}
		uint16_t value = d.type1.data & 0x0FFF;
		if(value>handle->rawThreshold){
			if((!set)||isOn){
				isOn = false;
                if (handle->callback) handle->callback(false);
			}
		}
		else{
			if((!set)||(!isOn)){
				isOn = true;
                if (handle->callback) handle->callback(true);
			}
		}
		set = true;
	}
	delete handle;
}
void AnalogueInputs::errorCheck(esp_err_t err){
	if (err == ESP_OK) return;
	char theMessage[64];
	esp_err_to_name_r(err, theMessage, sizeof(theMessage));
	Aborter::safeAbort(TAG, "Fatal ADC error: %s", theMessage);
}
void AnalogueInputs::measureNReadsPerSecond() {
	adc_digi_output_data_t d; // Stores a single read value
	uint32_t len = 0;
	while(true){
		// drain backlog
		int nCycles = 0;
		
		uint64_t startTime = esp_timer_get_time();
		while (nCycles<10000) {
			esp_err_t err = adc_continuous_read(
				_adc_hdl,                              // handle
				reinterpret_cast<uint8_t*>(&d),        // buffer
				4,                             // size
				&len,                                   // out bytes
				0                                       // non-blocking
			); 
			if(err!= ESP_OK){
				continue;
			}
			if(len!=4){
				continue;
			}
			nCycles++;
		}
		uint64_t delay  = esp_timer_get_time() - startTime;
		uint64_t readsPerSecond = (10000ULL*1000000ULL)/delay;
		Log::Info(TAG, "Achieving %"PRIu64 " reads per second", readsPerSecond);
	}
}

