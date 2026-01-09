
/* ========================== AnalogueInputs.cpp ============================ */
#include "ADC.hpp"
#include "esp_log.h"
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "../Timing/Delay.hpp"
#include "../Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "esp_timer.h"
#include "../Tasks/TaskFactory.hpp"
#include "../Tasks/TaskFactory.hpp"
#include "../Core/CleanupBucket.hpp"
#include "Timing/TimeHelper.hpp"


const char* ADC::TAG = "ADC";
bool  ADC::_initialized    = false;
ADC* ADC::_instance = new ADC();
std::atomic<bool> ADC::_inUse(false);
adc_continuous_handle_t ADC::_adc_hdl = nullptr;
esp_adc_cal_characteristics_t* ADC::_adc_chars = nullptr;
float ADC::_correctionFactor = 1.0;
float ADC::_voltageToRaw     = 0.0;
std::optional<adc_channel_t> ADC::_currentChannel = std::nullopt;
ReverseVoltageToRawLookup* ADC::_reverseLookup = nullptr;
std::mutex ADC::_mutexSetChannel;

void ADC::initialize()
{
    if (_initialized) {
        LOG_WARN("Already initialized"); // (tag, warning format...)
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
void ADC::setChannel(adc_channel_t ch)
{
	std::unique_lock<std::mutex> lock(_mutexSetChannel);
    if (_currentChannel && _currentChannel.value() == ch) return;   // already on it

    // Stop the driver if running
    if (_adc_hdl) {
        esp_err_t err = adc_continuous_stop(_adc_hdl); // (adc_continuous_handle_t handle)
		if((err!=ESP_OK)&&(err!=ESP_ERR_INVALID_STATE)){
			errorCheck(err);
		}
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

    _currentChannel = ch;
}
void ADC::use(adc_channel_t ch, const std::function<void(IADCSession&&)>& fn) {
    bool expected = false;
    if (!_inUse.compare_exchange_strong(expected, true)) {
        SAFE_ABORT("ADC::use() called while already in use");
        return;
    }
    CleanupBucket cleanup;
    cleanup.addCallback([&]() noexcept {
        _inUse.store(false);
        stop();
    });
    _instance->setChannel(ch);  // Set desired channel first
    start();
    ADCSession proxy(static_cast<IADCSession*>(_instance));
    fn(std::move(proxy));  // pass it to closure
    // Invalidate after call
    proxy.invalidate();  // ensures no external code kept it
}

void ADC::start(){
    errorCheck(adc_continuous_start(_adc_hdl)); // (adc_continuous_handle_t handle)
}
void ADC::stop(){
	esp_err_t err = adc_continuous_stop(_adc_hdl);
	if((err==ESP_OK)||(err==ESP_ERR_INVALID_STATE)){
		return;
	}
    errorCheck(err); // (adc_continuous_handle_t handle)
}
// -------------------- RAW sample helpers ---------------------------------
uint16_t ADC::singleRawLatestSampleSelectedChannel()
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

uint16_t ADC::averagedRawSampleSelectedChannel(int nSamples)
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
float ADC::convertRawToVoltage(uint16_t raw)
{
    float mv = esp_adc_cal_raw_to_voltage(
        raw,        // raw ADC reading
        _adc_chars  // pointer to previously characterized calibration data
    );
    return (mv * _correctionFactor) / 1000.0f; // convert mV to V
}


uint16_t ADC::convertVoltageToApproximateRaw(float voltage)
{
	return (uint16_t)_reverseLookup->lookupVolts(voltage);
}

// ---------------- public wrap‑ups ----------------------------------------
float ADC::singleCorrectedVoltageSampleSelectedChannel()
{
    return convertRawToVoltage(singleRawLatestSampleSelectedChannel());
}

float ADC::averagedCorrectedVoltageSampleSelectedChannel(int nSamples)
{
    return convertRawToVoltage(averagedRawSampleSelectedChannel(nSamples));
}

float ADC::getCorrection()
{
    return _correctionFactor;
}
float ADC::getMinimumVoltageCanRead(){
	return convertRawToVoltage(0);
}
std::shared_ptr<MonitorVoltageThresholdHandle> 
	ADC::monitorVoltageThresholdWithNewPriorityTask(
		adc_channel_t channel,
		float initialVoltage, 
		std::function<void(bool)> callback
	) {
	if(!callback){
		SAFE_ABORT("No callback provided!");
		return nullptr;
	}
	std::shared_ptr<MonitorVoltageThresholdHandle> handle = 
		std::make_shared<MonitorVoltageThresholdHandle>(
			channel, initialVoltage, _reverseLookup,
			std::move(callback)
	);
	
	TaskFactory::createPriorityTask<MonitorVoltageThresholdHandle>(
		&ADC::_monitorVoltageThreshold,
		handle,
		"ADC::monitorVoltageThreshold"
    );
	return handle;
}
void ADC::_monitorVoltageThreshold(std::shared_ptr<MonitorVoltageThresholdHandle> handle) {
	use(handle->getChannel(), [handle](IADCSession&& adc){
		adc_digi_output_data_t d; // Stores a single read value
		uint32_t len = 0;
		esp_err_t err;
		bool set = false;
		bool currentReached = false;
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
			//LOG_INFO("value was: %d ", value);
			//LOG_INFO("raw threshold was: %d ", handle->getRawThreshold());
			if(value<handle->getRawThreshold()){
				if((!set)||currentReached){
					currentReached = false;
					handle->callback(false);
				}
			}
			else{
				if((!set)||(!currentReached)){
					currentReached = true;
					handle->callback(true);
				}
			}
			set = true;
			handle->setVoltageRaw(value);
		}
	});
}


std::shared_ptr<IMonitorCurrentAndPowerHandle> 
ADC::monitorCurrentAndPower(
	float senseResistanceOhms, 
	float outputCurrentLimitingResistanceOhms,
	float cumulativeEnergyThresholdJ,
	float energyDisipatedJPerS,
	std::function<void(bool)> callback
) {
	if (!callback){
		SAFE_ABORT("No callback provided!");
		return nullptr;
	} 

	std::shared_ptr<MonitorCurrentAndPowerHandle> handle = std::make_shared<MonitorCurrentAndPowerHandle>(
		senseResistanceOhms, 
		outputCurrentLimitingResistanceOhms, 
		cumulativeEnergyThresholdJ,
		energyDisipatedJPerS,
		std::move(callback)
	);

	TaskFactory::createPriorityTask<MonitorCurrentAndPowerHandle>(
		&ADC::_monitorCurrentAndPower,
		handle,
		"ADC::monitorCurrentAndPower"
	);

	// Cast to interface type before returning
	return std::static_pointer_cast<IMonitorCurrentAndPowerHandle>(handle);
}

void ADC::_monitorCurrentAndPower(
	std::shared_ptr<MonitorCurrentAndPowerHandle> handle
){
	esp_err_t err;
	float cumulativeEnergyUj = 0;
	float cumulativeEnergyThresholdUj = handle->getCumulativeEnergyThresholdJ() * 1000.0;
	uint64_t lastTimeUs = esp_timer_get_time();
	uint64_t nextTimeUs;
	uint64_t dTUs;
	float currentA, powerW;
	bool set = false;
	bool isOn = false;
	adc_digi_output_data_t d; // Stores a single read value
	uint32_t len = 0;
	while(!handle->getExitFlag()){
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
		nextTimeUs = esp_timer_get_time();
		dTUs = nextTimeUs - lastTimeUs;
		if(dTUs<=0){
			continue;
		}
		currentA = convertRawToVoltage(d.type1.data & 0x0FFF) / handle->getSenseResistanceOhms();
		handle->setCurrentA(currentA);
		powerW = currentA * currentA * handle->getOutputCurrentLimitingResistanceOhms();
		cumulativeEnergyUj += powerW * dTUs;
		cumulativeEnergyUj -= static_cast<float>(dTUs) * handle->getEnergyDisipatedUjPerUs();
		if(cumulativeEnergyUj<0){
			cumulativeEnergyUj =0;
		}
		if(cumulativeEnergyUj>= cumulativeEnergyThresholdUj){
			if((!set)||isOn){
				isOn = false;
                handle->callbackReachedThreshold(false);
			}
		}
		else{
			if((!set)||(!isOn)){
				isOn = true;
                handle->callbackReachedThreshold(true);
			}
		}
		lastTimeUs = nextTimeUs;
	}
}



bool ADC::getRawQuickly(uint16_t& value){
	adc_digi_output_data_t d;
	uint32_t len = 0;
	esp_err_t err = adc_continuous_read(
		_adc_hdl,                                // handle
		reinterpret_cast<uint8_t*>(&d),          // buffer
		4,                             // size
		&len,                                     // out: bytes read
		0                            // timeout (wait forever)
	);
	if(err!= ESP_OK){
		return false;
	}
	if(len!=4){
		return false;
	}
	value =  d.type1.data & 0x0FFF;
	return true;
}

float ADC::getVoltage(){
	adc_digi_output_data_t d;
	uint32_t len = 0;
	esp_err_t err;
	while(true){
		err = adc_continuous_read(
			_adc_hdl,                                // handle
			reinterpret_cast<uint8_t*>(&d),          // buffer
			4,                             // size
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
void ADC::errorCheck(esp_err_t err){
	if (err == ESP_OK) return;
	char theMessage[64];
	esp_err_to_name_r(err, theMessage, sizeof(theMessage));
	SAFE_ABORT("Fatal ADC error: %s", theMessage);
}
void ADC::measureNReadsPerSecond() {
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
		LOG_INFO("Achieving %"PRIu64 " reads per second", readsPerSecond);
	}
}

