#ifndef I2C_CONFIGURATION_HPP
#define I2C_CONFIGURATION_HPP

#include <cstdint>
#include <driver/gpio.h>  // gpio_num_t, GPIO_NUM_*
#include <driver/i2c.h>   // i2c_port_t, I2C_NUM_*

#define I2C_MASTER_NUM_DEFAULT             I2C_NUM_0
#define I2C_MASTER_SCL_IO_DEFAULT          GPIO_NUM_22
#define I2C_MASTER_SDA_IO_DEFAULT          GPIO_NUM_21
#define I2C_MASTER_FREQ_HZ_DEFAULT         100000u
#define I2C_MASTER_PULLUPS_ENABLED_DEFAULT false
struct I2CConfigurationOptions {
    i2c_port_t port     = I2C_MASTER_NUM_DEFAULT;
    gpio_num_t sdaPin   = I2C_MASTER_SDA_IO_DEFAULT;
    gpio_num_t sclPin   = I2C_MASTER_SCL_IO_DEFAULT;
    uint32_t   clkSpeed = I2C_MASTER_FREQ_HZ_DEFAULT;
    bool       pullups  = I2C_MASTER_PULLUPS_ENABLED_DEFAULT;
};

class I2CConfiguration {
public:
    I2CConfiguration(i2c_port_t port   = I2C_MASTER_NUM_DEFAULT,
                     gpio_num_t sdaPin = I2C_MASTER_SDA_IO_DEFAULT,
                     gpio_num_t sclPin = I2C_MASTER_SCL_IO_DEFAULT,
                     uint32_t   clkSpeedHz  = I2C_MASTER_FREQ_HZ_DEFAULT,
                     bool       pullupsEnabled = I2C_MASTER_PULLUPS_ENABLED_DEFAULT);


    // 2) “Named” ctor via options struct (use designated initializers)
    explicit I2CConfiguration(const I2CConfigurationOptions& opts);
	
    I2CConfiguration(int port,
                     int sdaPin,
                     int sclPin,
                     int clkSpeedHz,
                     bool pullupsEnabled);

    // Getters
    i2c_port_t  getPort()           const { return _port; }
    gpio_num_t  getSDAPin()         const { return _sdaPin; }
    gpio_num_t  getSCLPin()         const { return _sclPin; }
    uint32_t    getClkSpeedHz()       const { return _clkSpeedHz; }
    bool        getPullupsEnabled() const { return _pullupsEnabled; }

private:
    i2c_port_t  _port;
    gpio_num_t  _sdaPin;
    gpio_num_t  _sclPin;
    uint32_t    _clkSpeedHz;
    bool        _pullupsEnabled;
};

#endif // I2C_CONFIGURATION_HPP
