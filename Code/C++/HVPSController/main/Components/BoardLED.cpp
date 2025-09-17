#include "BoardLED.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

BoardLED* BoardLED::instance = nullptr;
const gpio_num_t BoardLED::ledPin = GPIO_NUM_2;
const char* BoardLED::TAG = "BoardLED";

// Private constructor to initialize the LED pin
BoardLED::BoardLED() {
    gpio_reset_pin(ledPin);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
}

// Initialize the singleton instance of the BoardLED class
void BoardLED::initialize() {
    if (instance == nullptr) {
        instance = new BoardLED();
    }
}

// Get the singleton instance of the BoardLED class
BoardLED& BoardLED::getInstance() {
    if (instance == nullptr) {
        ESP_LOGE(TAG, "BoardLED not initialized. Call initialize() first.");
        // Optionally, you could throw an exception here if exceptions are enabled
    }
    return *instance;
}

// Function to turn the LED on
esp_err_t BoardLED::on() {
    gpio_set_level(ledPin, 1);
    return ESP_OK;
}

// Function to turn the LED off
esp_err_t BoardLED::off() {
    gpio_set_level(ledPin, 0);
    return ESP_OK;
}