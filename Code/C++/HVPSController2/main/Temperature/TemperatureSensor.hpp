#pragma once

#include "Core/Macros.hpp"
#include "Communication/Interfaces/IReadWrite.hpp"
#include "Components/TemperatureSensors/Interfaces/ITemperatureSensor.hpp"
#include "Components/TemperatureSensors/LM75A.hpp"
#include "Core/SingletonBase.hpp"
#include "esp_err.h"
#include <string>

template<class Derived>
class TemperatureSensor
    : public SingletonBase<Derived>
    , public ITemperatureSensor
{
private:
    LM75A _lm75a;

protected:
    TemperatureSensor(IReadWrite& readWrite,
                      uint8_t address,
                      std::string name)
        : _lm75a(readWrite, address, std::move(name))
    {}

private:
    friend class SingletonBase<Derived>;
    DISALLOW_COPY_MOVE(TemperatureSensor);

public:
    static const char* getTag() {
        return Derived::TAG;
    }

    std::string getName() const override {
        return _lm75a.getName();
    }

    esp_err_t getTemperature(float& temperature) override {
        return _lm75a.getTemperature(temperature);
    }
};
