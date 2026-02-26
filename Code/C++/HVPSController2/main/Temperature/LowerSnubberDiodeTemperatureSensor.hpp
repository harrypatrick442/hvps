#pragma once

#include "TemperatureSensor.hpp"
#include "Communication/Interfaces/IReadWrite.hpp"

class LowerSnubberDiodeTemperatureSensor final 
	: public TemperatureSensor<LowerSnubberDiodeTemperatureSensor>
{
public:
    static constexpr const char* TAG = "LowerSnubberDiodeTemperatureSensor";

private:
    friend class SingletonBase<LowerSnubberDiodeTemperatureSensor>;
    LowerSnubberDiodeTemperatureSensor(IReadWrite& readWrite);
};
