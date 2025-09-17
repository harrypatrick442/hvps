#ifndef BOARD_LED_H
#define BOARD_LED_H

#include "driver/gpio.h"
#include "esp_log.h"

class BoardLED {
public:
    // Initialize the singleton instance of the BoardLED class
    static void initialize();

    // Get the singleton instance of the BoardLED class
    static BoardLED& getInstance();

    // Function to turn the LED on
    esp_err_t on();

    // Function to turn the LED off
    esp_err_t off();

private:
    // Private constructor to prevent instantiation
    BoardLED();

    // Delete copy constructor and assignment operator
    BoardLED(const BoardLED&) = delete;
    BoardLED& operator=(const BoardLED&) = delete;

    static BoardLED* instance;
    static const gpio_num_t ledPin;
    static const char* TAG;
};

#endif // BOARD_LED_H