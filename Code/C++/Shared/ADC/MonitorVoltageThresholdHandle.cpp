#include "MonitorVoltageThresholdHandle.hpp"
#include "ADC.hpp"
MonitorVoltageThresholdHandle::MonitorVoltageThresholdHandle(
    adc_channel_t channel,
    float initialThresholdVoltage,
    ReverseVoltageToRawLookup* reverseLookup,
    std::function<void(bool)> cb
)
    : _reverseLookup(reverseLookup),
      _channel(channel),
      _currentVoltageRaw(0),
      _rawThreshold(0),
      exit(false),
      callback(std::move(cb))
{
    setThresholdVoltage(initialThresholdVoltage);
}

adc_channel_t MonitorVoltageThresholdHandle::getChannel() {
    return _channel;
}

void MonitorVoltageThresholdHandle::setThresholdVoltage(float voltage) {
    _rawThreshold.store(_reverseLookup->lookupVolts(voltage),
                       std::memory_order_relaxed);
}

void MonitorVoltageThresholdHandle::setVoltageRaw(uint16_t raw) {
    _currentVoltageRaw.store(raw, std::memory_order_relaxed);
}

float MonitorVoltageThresholdHandle::getVoltage() {
    uint16_t raw = _currentVoltageRaw.load(std::memory_order_relaxed);
	return ADC::convertRawToVoltage(raw);
}
uint16_t MonitorVoltageThresholdHandle::getRawThreshold() {
    uint16_t raw = _rawThreshold.load(std::memory_order_relaxed);
	return raw;
}

void MonitorVoltageThresholdHandle::stop() {
    exit.store(true, std::memory_order_relaxed);
}
