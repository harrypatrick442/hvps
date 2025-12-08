#include "ISRSoftwareUART.hpp"
#include "esp_timer.h"
#include <rom/ets_sys.h>
#include "Logging/Log.hpp"
#include "Tasks/TaskFactory.hpp"
ISRSoftwareUART::ISRSoftwareUART(
    int nUART,
    int txPin,
    int rxPin,
    int baudRate,
    bool invertTx,
    bool invertRx
)
: UARTBase(nUART, txPin, rxPin, baudRate, invertTx, invertRx),
  _bitPeriodUs(1000000 / baudRate),
  _halfBitPeriodUs(500000 / baudRate),
  _txGPIONum((gpio_num_t)txPin),
  _rxGPIONum((gpio_num_t)rxPin),
  _rxBuffer(2048)
{
    // Nothing else here — configure() handles everything.
}
bool ISRSoftwareUART::configure() {
    if (!checkNUARTValid(_nUART))
        return false;

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
	return TaskFactory::createNonPriorityTask(
		[this](){
			readLooper();
		}, 
		"ISRSoftwareUART::readLooper"
	);
}

size_t ISRSoftwareUART::writeBytes(const char* src, size_t len) {
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

    return len;
}
size_t ISRSoftwareUART::readBytes(char* dst, size_t maxlen, uint32_t timeoutMs)
{
    if (maxlen == 0) return 0;

    uint64_t deadlineUs = esp_timer_get_time() + (uint64_t)timeoutMs * 1000ULL;
    size_t totalRead = 0;

    while (totalRead < maxlen)
    {
        // Try to take any available bytes immediately
        size_t taken = _rxBuffer.take((uint8_t*)(dst + totalRead), maxlen - totalRead);

        if (taken > 0) {
            totalRead += taken;
            continue;               // try for more until maxlen reached
        }

        // No data in buffer → check timeout
        if (esp_timer_get_time() >= deadlineUs) {
            break; // timeout expired
        }

        // Light delay to avoid tight spin-loop (1 tick or ~1ms)
        vTaskDelay(1);
    }

    return totalRead;
}

void ISRSoftwareUART::readLooper()
{
    while (true) {
        // ----- 1) WAIT FOR START BIT -----
        // RX idle = HIGH → start bit = LOW
        int level;
        uint64_t tStart;

        while (true) {
            level = gpio_get_level(_rxGPIONum) ^ _invertRx;

            if (level == 0) {
                // Detected falling edge: START BIT
                tStart = esp_timer_get_time();
                break;
            }

            // No start bit — just keep scanning indefinitely
            // (This is the behaviour you asked for)
        }

        // ----- 2) SAMPLE THE BYTE -----
        // First data bit center: tStart + 1.5 * bitPeriod
        uint64_t sampleTime = tStart + _bitPeriodUs + (_bitPeriodUs / 2);

        uint8_t byte = 0;

        for (int bit = 0; bit < 8; bit++) {

            // wait until sampleTime
            while (esp_timer_get_time() < sampleTime) {
                // tight spin until sample moment
            }

            int bitVal = gpio_get_level(_rxGPIONum) ^ _invertRx;
            byte |= (bitVal << bit);

            sampleTime += _bitPeriodUs; // next bit
        }

        // ----- 3) STOP BIT CHECK (optional) -----
        // Sample stop bit at its center
        while (esp_timer_get_time() < sampleTime) {
            // spin
        }

        int stop = gpio_get_level(_rxGPIONum) ^ _invertRx;
        if (stop == 0) {
            // framing error — optional:
            // continue;  // ignore byte
            // but we will keep the byte to maximize robustness
        }

        // ----- 4) PUSH BYTE INTO RING BUFFER -----
        _rxBuffer.push(&byte, 1);

        // Continue immediately to detect the next start bit
    }
}


void ISRSoftwareUART::flushTx() { }
