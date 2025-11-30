#include "SoftwareUART.hpp"
#include "esp_timer.h"
#include <rom/ets_sys.h>
SoftwareUART::SoftwareUART(
    int nUART,
    int txPin,
    int rxPin,
    int baudRate,
    bool invertTx,
    bool invertRx
)
: UARTBase(nUART, txPin, rxPin, baudRate, invertTx, invertRx),
  _bitPeriodUs(1000000 / baudRate),
  _txGPIONum((gpio_num_t)txPin),
  _rxGPIONum((gpio_num_t)rxPin)
{
    // Nothing else here — configure() handles everything.
}
bool SoftwareUART::configure() {
    if (!checkNUARTValid(_nUART))
        return false;

    // Precompute bit timing
    _bitPeriodUs = 1000000 / _baudRate;

    // Configure TX pin
    gpio_config_t io_conf_tx = {};
    io_conf_tx.pin_bit_mask = (1ULL << _txPin);
    io_conf_tx.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf_tx);

    // Default idle state = HIGH
    gpio_set_level(_txGPIONum, _invertTx ? 0 : 1);

    // Configure RX pin
    gpio_config_t io_conf_rx = {};
    io_conf_rx.pin_bit_mask = (1ULL << _rxPin);
    io_conf_rx.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf_rx);

    return true;
}

int SoftwareUART::writeBytes(const char* src, size_t len) {
    for (size_t i = 0; i < len; i++) {
        unsigned char b = src[i];

        // Start bit (LOW)
        gpio_set_level(_txGPIONum, _invertTx ? 1 : 0);
        ets_delay_us(_bitPeriodUs);

        // Data bits (LSB first)
        for (int bit = 0; bit < 8; bit++) {
            int bitval = (b >> bit) & 1;
            bitval ^= _invertTx;  // invert if needed
            gpio_set_level(_txGPIONum, bitval);
            ets_delay_us(_bitPeriodUs);
        }

        // Stop bit (HIGH)
        gpio_set_level(_txGPIONum, _invertTx ? 0 : 1);
        ets_delay_us(_bitPeriodUs);
    }

    return (int)len;
}

int SoftwareUART::readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) {
    int count = 0;
    int64_t deadlineUs = esp_timer_get_time() + (timeoutMs * 1000);

    while (count < (int)maxlen) {
        // Wait for start bit (line goes LOW)
        while ((esp_timer_get_time() < deadlineUs)) {
            int level = gpio_get_level(_rxGPIONum);
            level ^= _invertRx;
            if (level == 0) break;
        }

        if (esp_timer_get_time() >= deadlineUs)
            return count; // no more bytes

        // We detected start bit → wait half bit to sample center
        ets_delay_us(_bitPeriodUs / 2);

        unsigned char byte = 0;

        for (int bit = 0; bit < 8; bit++) {
            ets_delay_us(_bitPeriodUs);
            int bitval = gpio_get_level(_rxGPIONum);
            bitval ^= _invertRx;
            byte |= (bitval << bit);
        }

        // Stop bit window
        ets_delay_us(_bitPeriodUs);

        dst[count++] = byte;
    }

    return count;
}

void SoftwareUART::flushTx() { }
