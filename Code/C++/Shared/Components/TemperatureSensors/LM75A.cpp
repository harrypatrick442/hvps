#include "LM75A.hpp"
#include "esp_log.h"

static const char *TAG = "LM75A";

LM75A::LM75A(IReadWrite& readWrite, uint8_t address) : readWrite(readWrite), address(address) {
    // Initialization code if needed
}

LM75A::~LM75A() {
    // Destructor code if needed
}

std::string LM75A::GetName() const {
    return "LM75A";
}

esp_err_t LM75A::GetTemperature(float& temperature) {
    uint8_t data[2];
    esp_err_t err = readWrite.readRegister(address, TEMPERATURE_REGISTER, data, 2);
    if (err != ESP_OK) {
        return err;
    }
    int16_t raw_temp = (data[0] << 8) | data[1];
    raw_temp >>= 5; // LM75A temperature data is 11-bit
    temperature = raw_temp * 0.125f; // Each bit represents 0.125 degrees Celsius
    return ESP_OK;
}