#pragma once
class IGetCurrentVoltage {
public:
    virtual ~IGetCurrentVoltage() = default;

    virtual float getVoltage() = 0;
};