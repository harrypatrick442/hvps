#include "I2C.hpp"
#include "driver/i2c.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "../../Logging/Log.hpp"
#define I2C_MASTER_TIMEOUT_MS 1000
#include <utility>
const char* I2C::TAG = "I2C";
I2C* I2C::_instance = nullptr;

// Private constructor to initialize the I2C driver
I2C::I2C(I2CConfiguration configuration) : 
	_configuration(std::move(configuration)),
	_port(_configuration.getPort())
	{
	auto pullupEn = _configuration.getPullupsEnabled()?GPIO_PULLUP_ENABLE:GPIO_PULLUP_DISABLE;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = _configuration.getSDAPin();
    conf.sda_pullup_en = pullupEn;
    conf.scl_io_num = _configuration.getSCLPin();
    conf.scl_pullup_en = pullupEn;
    conf.master.clk_speed = _configuration.getClkSpeedHz();
	
    i2c_param_config(_port, &conf);
	
    #ifndef I2C_MASTER_RX_BUF_DISABLE
    #define I2C_MASTER_RX_BUF_DISABLE 0
    #endif
    #ifndef I2C_MASTER_TX_BUF_DISABLE
    #define I2C_MASTER_TX_BUF_DISABLE 0
    #endif
	
    i2c_param_config(_port, &conf);

    esp_err_t err = i2c_driver_install(_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (err != ESP_OK) {
        Aborter::safeAbort(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
    }
}

// Initialize the singleton instance of the I2C class
void I2C::initialize(
    I2CConfiguration configuration) {
    if (_instance == nullptr) {
        _instance = new I2C(configuration);
    }
}

// Get the singleton instance of the I2C class
I2C& I2C::getInstance() {
    if (_instance == nullptr) {
        Aborter::safeAbort(TAG, "I2C not initialized. Call initialize() first.");
        // Optionally, you could throw an exception here if exceptions are enabled
    }
    return *_instance;
}

// Function to write to an I2C device
esp_err_t I2C::write(uint8_t i2c_addr, uint8_t *data_wr, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data_wr, size, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(_port, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Function to read from an I2C device
esp_err_t I2C::read(uint8_t i2c_addr, uint8_t *data_rd, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data_rd, size, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(_port, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Function to read a register from an I2C device
esp_err_t I2C::readRegister(uint8_t i2c_addr, uint8_t reg, uint8_t *data_rd, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1) | I2C_MASTER_READ, true);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}
void I2C::scanAndLog() {
    Log::Info(TAG, "I2C Scanner starting...");

    for (uint8_t address = 1; address < 127; ++address) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(_port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK) {
            Log::Info(TAG, "I2C device found at address 0x%02X", address);
        } else if (ret == ESP_ERR_TIMEOUT) {
            Log::Warn(TAG, "I2C device at address 0x%02X not responding", address);
        }
    }

    ESP_LOGI(TAG, "I2C Scanner finished.");
}