#ifndef MPU6050_H
#define MPU6050_H

#include "../../Communication/Interfaces/IReadWrite.hpp"
#include "esp_err.h"
#include <string>
#include <cstring>

class MPU6050 {
public:
    MPU6050(IReadWrite& readWrite, uint8_t address=0x68);
    ~MPU6050();

    std::string GetName() const;
    esp_err_t Initialize();
    esp_err_t GetAcceleration(float& ax, float& ay, float& az);
    esp_err_t GetGyroscope(float& gx, float& gy, float& gz);

private:
    IReadWrite& readWrite;
    uint8_t address;

    esp_err_t readRegister(uint8_t reg, uint8_t* data, size_t len);
    esp_err_t writeRegister(uint8_t reg, uint8_t* data, size_t len);

    static constexpr uint8_t WHO_AM_I_REGISTER = 0x75;
    static constexpr uint8_t PWR_MGMT_1_REGISTER = 0x6B;
    static constexpr uint8_t ACCEL_XOUT_H_REGISTER = 0x3B;
    static constexpr uint8_t GYRO_XOUT_H_REGISTER = 0x43;
};

#endif // MPU6050_H