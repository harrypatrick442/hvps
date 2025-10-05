#ifndef I2C_H
#define I2C_H

#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "../Interfaces/IReadWrite.hpp"
#include "I2CConfiguration.hpp"
class I2C : public IReadWrite {
public:
    // Initialize the singleton instance of the I2C class
    static void initialize(I2CConfiguration configuration);

    // Get the singleton instance of the I2C class
    static I2C& getInstance();

    // Function to write to an I2C device
    esp_err_t write(uint8_t i2c_addr, uint8_t *data_wr, size_t size) override;

    // Function to read from an I2C device
    esp_err_t read(uint8_t i2c_addr, uint8_t *data_rd, size_t size) override;
    
     // Function to read a register from an I2C device
    esp_err_t readRegister(uint8_t i2c_addr, uint8_t reg, uint8_t *data_rd, size_t size) override;
    void scanAndLog();

private:
    // Private constructor to prevent instantiation
    I2C(
        I2CConfiguration configuration);

    // Delete copy constructor and assignment operator
    I2C(const I2C&) = delete;
    I2C& operator=(const I2C&) = delete;

    static I2C* _instance;
	I2CConfiguration _configuration;
    i2c_port_t _port;
    static const char* TAG;
};

#endif // I2C_H