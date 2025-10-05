#include "MPU6050.hpp"
#include "esp_log.h"
#include <cstring>
#include "../../Logging/Log.hpp"
static const char *TAG = "MPU6050";

MPU6050::MPU6050(IReadWrite& readWrite, uint8_t address) : readWrite(readWrite), address(address) {
    // Initialization code if needed
}

MPU6050::~MPU6050() {
    // Destructor code if needed
}

std::string MPU6050::GetName() const {
    return "MPU6050";
}

esp_err_t MPU6050::Initialize() {
    uint8_t data = 0x00; // Wake up the MPU6050
    return writeRegister(PWR_MGMT_1_REGISTER, &data, 1);
}

esp_err_t MPU6050::GetAcceleration(float& ax, float& ay, float& az) {
    uint8_t data[6];
    esp_err_t err = readRegister(ACCEL_XOUT_H_REGISTER, data, 6);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read acceleration data");
        return err;
    }

    int16_t raw_ax = (data[0] << 8) | data[1];
    int16_t raw_ay = (data[2] << 8) | data[3];
    int16_t raw_az = (data[4] << 8) | data[5];

    ax = raw_ax / 16384.0f; // Assuming the accelerometer is set to ±2g
    ay = raw_ay / 16384.0f;
    az = raw_az / 16384.0f;

    return ESP_OK;
}

esp_err_t MPU6050::GetGyroscope(float& gx, float& gy, float& gz) {
    uint8_t data[6];
    esp_err_t err = readRegister(GYRO_XOUT_H_REGISTER, data, 6);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read gyroscope data");
        return err;
    }

    int16_t raw_gx = (data[0] << 8) | data[1];
    int16_t raw_gy = (data[2] << 8) | data[3];
    int16_t raw_gz = (data[4] << 8) | data[5];

    gx = raw_gx / 131.0f; // Assuming the gyroscope is set to ±250°/s
    gy = raw_gy / 131.0f;
    gz = raw_gz / 131.0f;

    return ESP_OK;
}

esp_err_t MPU6050::readRegister(uint8_t reg, uint8_t* data, size_t len) {
    return readWrite.readRegister(address, reg, data, len);
}

esp_err_t MPU6050::writeRegister(uint8_t reg, uint8_t* data, size_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    memcpy(buffer + 1, data, len);
    return readWrite.write(address, buffer, len + 1);
}