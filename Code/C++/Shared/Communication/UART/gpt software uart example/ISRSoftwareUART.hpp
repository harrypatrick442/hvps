#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_attr.h"
#include "ISRSoftwareUART_Engine.hpp"

class ISRSoftwareUART
{
public:
    ISRSoftwareUART(int gpioRx, int gpioTx, bool invert=false);

    void begin(int baud);
    void writeByte(uint8_t b);
    bool readByte(uint8_t& out);
    bool available() const;

    // ISR handlers
    void IRAM_ATTR handleRxISR();
    void IRAM_ATTR handleTxISR();

private:
    void configurePins();

    int _gpioRx;
    int _gpioTx;
    bool _invert;

    // RX state machine
    bool rx_active;
    int rx_tickCount;
    int rx_bitIndex;
    uint8_t rx_byte;

    // TX state machine
    bool tx_active;
    int tx_tickCount;
    int tx_bitIndex;
    uint8_t tx_byte;
    volatile bool tx_hasByte;
    volatile uint8_t tx_buf;

    // RX buffer
    static constexpr int RX_BUF_SIZE = 128;
    uint8_t rx_buf[RX_BUF_SIZE];
    volatile int rx_head;
    volatile int rx_tail;
};
