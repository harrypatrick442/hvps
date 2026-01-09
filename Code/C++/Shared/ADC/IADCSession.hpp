#pragma once
#include <cstdint>       // For uint16_t
#include <memory>        // For std::shared_ptr
#include <functional>    // For std::function

#include "esp_adc/adc_continuous.h"   // For adc_channel_t
#include "MonitorVoltageThresholdHandle.hpp"   // For MonitorVoltageThresholdHandle
#include "IMonitorCurrentAndPowerHandle.hpp"   // For IMonitorCurrentAndPowerHandle
class IADCSession{
public:
    virtual ~IADCSession() = default;
	
    virtual uint16_t singleRawLatestSampleSelectedChannel() = 0;
    virtual uint16_t averagedRawSampleSelectedChannel(int nSamples = 32) = 0;

    virtual float   singleCorrectedVoltageSampleSelectedChannel() = 0;
    virtual float   averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32) = 0;

    virtual void setChannel(adc_channel_t ch) = 0;
    virtual float getCorrection() = 0;
    virtual float getVoltage() = 0;
	virtual bool getRawQuickly(uint16_t& value) = 0;
	virtual void measureNReadsPerSecond() = 0;
	virtual std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
		float senseResistanceOhms, 
		float outputCurrentLimitingResistanceOhms,
		float cumulativeEnergyThresholdJ,
		float energyDisipatedJPerS,
		std::function<void(bool)> callback
	) = 0;
};
