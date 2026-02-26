#pragma once
#include "driver/gpio.h"
class PinDefinitions{
public:
	static constexpr int OUTPUT_VOLTAGE_FEEDBACK_RX = GPIO_NUM_4;
	static constexpr int OUTPUT_VOLTAGE_FEEDBACK_TX = GPIO_NUM_17;
	static constexpr int OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_25;
	static constexpr int FIRST_STAGE_VOLTAGE_FEEDBACK_RX = GPIO_NUM_18;
	static constexpr int FIRST_STAGE_VOLTAGE_FEEDBACK_TX = GPIO_NUM_19;
	static constexpr int FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_5;
	static constexpr int OUTPUT_CURRENT_FEEDBACK_TX = GPIO_NUM_33;
	static constexpr int OUTPUT_CURRENT_FEEDBACK_RX = GPIO_NUM_32;
	static constexpr int OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED = GPIO_NUM_26;
	static constexpr int OTHER_PERIPHERALS_TX = GPIO_NUM_27;
	static constexpr int OTHER_PERIPHERALS_RX = GPIO_NUM_14;
	static constexpr int MOSFET_DRIVE = GPIO_NUM_23;
	static constexpr int PRIMARY_CURRENT_FEEDBACK = GPIO_NUM_34;
	static constexpr int POWER_SUPPLY_VOLTAGE_FEEDBACK = GPIO_NUM_35;
	static constexpr int SNUBBER_VOLTAGE_FEEDBACK = GPIO_NUM_36;
	static constexpr int REFERENCE_VOLTAGE = GPIO_NUM_39;
	static constexpr int SOFT_START_RESISTOR_BYPASS = GPIO_NUM_13;
	static constexpr int I2C_SDA_PIN = GPIO_NUM_21;
	static constexpr int I2C_SCL_PIN = GPIO_NUM_22;
};