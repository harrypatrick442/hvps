#ifndef LM75A_H
#define LM75A_H

#include "../../Communication/Interfaces/IReadWrite.hpp"
#include "esp_err.h"
#include <string>

class LM75A {
public:
    LM75A(IReadWrite& readWrite, uint8_t address=0x48);
    ~LM75A();

    std::string GetName() const;
    float GetTemperature(bool& successful);

private:
    IReadWrite& readWrite;
    uint8_t address;

    static constexpr uint8_t TEMPERATURE_REGISTER = 0x00;
    static constexpr uint8_t CONFIGURATION_REGISTER = 0x01;
    static constexpr uint8_t THYST_REGISTER = 0x02;
    static constexpr uint8_t TOS_REGISTER = 0x03;
};

#endif // LM75A_H