#pragma once
class IGetCurrentVoltage {
public:
    virtual ~IGetCurrentVoltage() = default;

    virtual double getVoltage() = 0;
};