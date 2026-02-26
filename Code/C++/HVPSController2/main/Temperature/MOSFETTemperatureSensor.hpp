#pragma once

#include "TemperatureSensor.hpp"
#include "Communication/Interfaces/IReadWrite.hpp"

class MOSFETTemperatureSensor final
    : public TemperatureSensor<MOSFETTemperatureSensor>
{
public:
    static constexpr const char* TAG = "MOSFETTemperatureSensor";

private:
    friend class SingletonBase<MOSFETTemperatureSensor>;
    MOSFETTemperatureSensor(IReadWrite& readWrite);
};
