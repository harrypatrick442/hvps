#ifndef IREADWRITE_H
#define IREADWRITE_H

#include "esp_err.h"

class IReadWrite {
public:
    virtual ~IReadWrite() = default;

    // Pure virtual function to write to a device
    virtual esp_err_t write(uint8_t i2c_addr, uint8_t *data_wr, size_t size) = 0;

    // Pure virtual function to read from a device
    virtual esp_err_t read(uint8_t i2c_addr, uint8_t *data_rd, size_t size) = 0;
    
    virtual esp_err_t readRegister(uint8_t i2c_addr, uint8_t reg, uint8_t *data_rd, size_t size) = 0;

};

#endif // IREADWRITE_H