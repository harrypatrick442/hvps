#pragma once
#include "Components/TemperatureSensors/Interfaces/ITemperatureSensor.hpp"
struct TemperatureSensorAndLimit {
public:
    ITemperatureSensor& temperatureSensor;
    const float limitDegreesC;

    TemperatureSensorAndLimit(ITemperatureSensor& sensor,
                              float limitC);
};
