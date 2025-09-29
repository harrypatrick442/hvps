#include "PinDefinitions.hpp"
#include "driver/gpio.h"
const int PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_RX = GPIO_NUM_4;
const int PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_TX = GPIO_NUM_17;
const int PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_32;
const int PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_RX = GPIO_NUM_18;
const int PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_TX = GPIO_NUM_19;
const int PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_5;
const int PinDefinitions::OUTPUT_CURRENT_FEEDBACK_TX = GPIO_NUM_33;
const int PinDefinitions::OUTPUT_CURRENT_FEEDBACK_RX = GPIO_NUM_25;
const int PinDefinitions::OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_26;
const int PinDefinitions::PERIPHERALS_TX = GPIO_NUM_27;
const int PinDefinitions::PERIPHERALS_RX = GPIO_NUM_14;
const int PinDefinitions::MOSFET_DRIVE = GPIO_NUM_23;
const int PinDefinitions::PRIMARY_CURRENT_FEEDBACK = GPIO_NUM_34;
const int PinDefinitions::POWER_SUPPLY_VOLTAGE_FEEDBACK = GPIO_NUM_35;
const int PinDefinitions::SNUBBER_VOLTAGE_FEEDBACK = GPIO_NUM_36;
const int PinDefinitions::REFERENCE_VOLTAGE = GPIO_NUM_39;
const int PinDefinitions::SOFT_START_RESISTOR_BYPASS = GPIO_NUM_13;