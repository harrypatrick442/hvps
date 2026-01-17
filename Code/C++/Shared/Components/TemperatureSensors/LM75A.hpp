#ifndef LM75A_H
#define LM75A_H

#include "../../Communication/Interfaces/IReadWrite.hpp"
#include "Interfaces/ITemperatureSensor.hpp"
#include "esp_err.h"
#include <string>

class LM75A: public ITemperatureSensor{
public:
    LM75A(IReadWrite& readWrite, uint8_t address=0x00, std::string name = "LM75A");
    ~LM75A();

    std::string getName() const override;
    esp_err_t getTemperature(float& temperature) override;

private:
    IReadWrite& _readWrite;
    uint8_t _address;
	std::string _name;

    static constexpr uint8_t TEMPERATURE_REGISTER = 0x00;
    static constexpr uint8_t CONFIGURATION_REGISTER = 0x01;
    static constexpr uint8_t THYST_REGISTER = 0x02;
    static constexpr uint8_t TOS_REGISTER = 0x03;
};

#endif // LM75A_H