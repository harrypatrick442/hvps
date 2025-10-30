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

    virtual double   singleCorrectedVoltageSampleSelectedChannel() = 0;
    virtual double   averagedCorrectedVoltageSampleSelectedChannel(int nSamples = 32) = 0;

    virtual void setChannel(adc_channel_t ch) = 0;
    virtual double getCorrection() = 0;
    virtual double getVoltage() = 0;
	virtual void measureNReadsPerSecond() = 0;
	virtual std::shared_ptr<MonitorVoltageThresholdHandle> monitorVoltageThresholdWithNewPriorityTask(
		double initialVoltage, 
		std::function<void(bool)> callback
	) = 0;
	virtual std::shared_ptr<IMonitorCurrentAndPowerHandle> monitorCurrentAndPower(
		double senseResistanceOhms, 
		double outputCurrentLimitingResistanceOhms,
		double cumulativeEnergyThresholdJ,
		double energyDisipatedJPerS,
		std::function<void(bool)> callback
	) = 0;
};
