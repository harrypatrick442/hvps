#include "ISRSoftwareUART.hpp"

ISRSoftwareUART::ISRSoftwareUART(int gpioRx, int gpioTx, bool invert)
    : _gpioRx(gpioRx),
      _gpioTx(gpioTx),
      _invert(invert),
      rx_active(false),
      rx_tickCount(0),
      rx_bitIndex(0),
      rx_byte(0),
      tx_active(false),
      tx_tickCount(0),
      tx_bitIndex(0),
      tx_byte(0),
      tx_hasByte(false),
      tx_buf(0),
      rx_head(0),
      rx_tail(0)
{
    ISRSoftwareUART_Engine::registerInstance(this);
}

void ISRSoftwareUART::begin(int baud)
{
    ISRSoftwareUART_Engine::init(baud);
    configurePins();
}

void ISRSoftwareUART::configurePins()
{
    gpio_config_t io = {};
    io.intr_type = GPIO_INTR_DISABLE;

    io.pin_bit_mask = (1ULL << _gpioRx);
    io.mode = GPIO_MODE_INPUT;
    gpio_config(&io);

    io.pin_bit_mask = (1ULL << _gpioTx);
    io.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io);

    gpio_set_level((gpio_num_t)_gpioTx, _invert ? 0 : 1);
}

bool ISRSoftwareUART::available() const
{
    return rx_head != rx_tail;
}

bool ISRSoftwareUART::readByte(uint8_t& out)
{
    if (rx_head == rx_tail) return false;

    out = rx_buf[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
    return true;
}

void ISRSoftwareUART::writeByte(uint8_t b)
{
    tx_buf = b;
    tx_hasByte = true;
}

//
// ---------------------- RX ISR --------------------------
//

void IRAM_ATTR ISRSoftwareUART::handleRxISR()
{
    int rx = gpio_get_level((gpio_num_t)_gpioRx);
    if (_invert) rx = !rx;

    if (!rx_active)
    {
        // detect start bit
        if (rx == 0)
        {
            rx_active = true;
            rx_tickCount = 0;
            rx_bitIndex = -1;  // start bit state
            rx_byte = 0;
        }
        return;
    }

    rx_tickCount++;

    if (rx_tickCount == (ISRSoftwareUART_Engine::OVERSAMPLE / 2))
    {
        if (rx_bitIndex == -1)
        {
            // mid-sample of start bit must be 0
            if (rx != 0)
            {
                rx_active = false;
                return;
            }
            rx_bitIndex = 0;
        }
        else if (rx_bitIndex < 8)
        {
            // sample bit i
            rx_byte |= (rx << rx_bitIndex);
            rx_bitIndex++;
        }
        else
        {
            // stop bit (must be high)
            if (rx == 1)
            {
                rx_buf[rx_head] = rx_byte;
                rx_head = (rx_head + 1) % RX_BUF_SIZE;
            }
            rx_active = false;
        }
    }
    else if (rx_tickCount >= ISRSoftwareUART_Engine::OVERSAMPLE)
    {
        rx_tickCount = 0;
    }
}

//
// ---------------------- TX ISR --------------------------
//

void IRAM_ATTR ISRSoftwareUART::handleTxISR()
{
    if (!tx_active)
    {
        if (!tx_hasByte) return;

        tx_byte = tx_buf;
        tx_hasByte = false;
        tx_active = true;
        tx_tickCount = 0;
        tx_bitIndex = -1;

        // start bit
        gpio_set_level((gpio_num_t)_gpioTx, _invert ? 1 : 0);
        return;
    }

    tx_tickCount++;

    if (tx_tickCount == ISRSoftwareUART_Engine::OVERSAMPLE)
    {
        tx_tickCount = 0;

        if (tx_bitIndex == -1)
        {
            tx_bitIndex = 0;
        }
        else if (tx_bitIndex < 8)
        {
            int bit = (tx_byte >> tx_bitIndex) & 1;
            gpio_set_level((gpio_num_t)_gpioTx, _invert ? !bit : bit);
            tx_bitIndex++;
        }
        else
        {
            // stop bit
            gpio_set_level((gpio_num_t)_gpioTx, _invert ? 0 : 1);
            tx_active = false;
        }
    }
}
