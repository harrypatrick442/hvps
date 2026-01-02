#pragma once
class IVoltageSource {
public:
    virtual ~IVoltageSource() = default;

    virtual float getVoltage(uint16_t& raw) = 0;
};