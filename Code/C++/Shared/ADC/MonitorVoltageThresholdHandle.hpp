#ifndef MonitorVoltageThresholdHandle_HPP
#define MonitorVoltageThresholdHandle_HPP

#include "ReverseVoltageToRawLookup.hpp"
#include <atomic>
#include <functional>

class MonitorVoltageThresholdHandle {
private:
    ReverseVoltageToRawLookup* _reverseLookup;
    adc_channel_t _channel;

    // Thread-safe current voltage (atomic)
    std::atomic<uint16_t> _currentVoltageRaw;
    // Raw ADC threshold (12-bit). Thread-safe.
    std::atomic<uint32_t> _rawThreshold;

public:

    // True when monitor task should exit
    std::atomic<bool> exit { false };

    // Callback: true = above threshold, false = below
    std::function<void(bool)> callback;

    // Constructor
    MonitorVoltageThresholdHandle(
        adc_channel_t channel,
        float initialThresholdVoltage,
        ReverseVoltageToRawLookup* reverseLookup,
        std::function<void(bool)> cb
    );

    adc_channel_t getChannel();

    void setThresholdVoltage(float voltage);

    void setVoltageRaw(uint16_t voltage);
    float getVoltage(uint16_t& raw);
	uint16_t getRawThreshold();

    void stop();
};

#endif
