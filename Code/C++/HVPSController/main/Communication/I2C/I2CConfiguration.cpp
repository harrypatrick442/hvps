#include "I2CConfiguration.hpp"
#include <cassert>

// ---- local helpers ----
static inline bool valid_i2c_port(i2c_port_t p) {
    return p >= I2C_NUM_0 && p < I2C_NUM_MAX;
}
static inline bool valid_gpio(gpio_num_t g) {
    return g >= GPIO_NUM_0 && g < GPIO_NUM_MAX; // rejects GPIO_NUM_NC (-1)
}
static inline void validate(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t clk) {
    assert(valid_i2c_port(port) && "Invalid I2C port");
    assert(valid_gpio(sda)      && "Invalid SDA GPIO");
    assert(valid_gpio(scl)      && "Invalid SCL GPIO");
    assert(sda != scl           && "SDA and SCL should be different pins");
    assert(clk > 0              && "Clock speed must be > 0");
}

I2CConfiguration::I2CConfiguration(i2c_port_t port,
                                   gpio_num_t sdaPin,
                                   gpio_num_t sclPin,
                                   uint32_t   clkSpeedHz,
                                   bool       pullupsEnabled)
: _port(port),
  _sdaPin(sdaPin),
  _sclPin(sclPin),
  _clkSpeedHz(clkSpeedHz),
  _pullupsEnabled(pullupsEnabled)
{
    validate(_port, _sdaPin, _sclPin, _clkSpeedHz);
}

// 2) Options-struct ctor (“named parameters” feel)
I2CConfiguration::I2CConfiguration(const I2CConfigurationOptions& opts)
: I2CConfiguration(opts.port, opts.sdaPin, opts.sclPin, opts.clkSpeed, opts.pullups) {}

// 3) Int convenience ctor (delegates to typed)
I2CConfiguration::I2CConfiguration(int port,
                                   int sdaPin,
                                   int sclPin,
                                   int clkSpeedHz,
                                   bool pullupsEnabled)
: I2CConfiguration(static_cast<i2c_port_t>(port),
                   static_cast<gpio_num_t>(sdaPin),
                   static_cast<gpio_num_t>(sclPin),
                   static_cast<uint32_t>(clkSpeedHz),
                   pullupsEnabled) {}
