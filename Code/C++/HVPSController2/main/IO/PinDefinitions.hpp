#pragma once
#include "driver/gpio.h"
class PinDefinitions{
public:
	static constexpr int LEDS_D_OUT = GPIO_NUM_4;
	static constexpr int POWER_SUPPLY_VOLTAGE_FEEDBACK = GPIO_NUM_36;
	static constexpr int SOFT_START_RESISTOR_BYPASS = GPIO_NUM_21;
	static constexpr int FAN_RELAY = GPIO_NUM_32;
	
	static constexpr int I2C_SDA_PIN = GPIO_NUM_17;
	static constexpr int I2C_SCL_PIN = GPIO_NUM_13;
	
	static constexpr int FPGA_OUT_SHIFT = GPIO_NUM_33;//FPGA PIN 60[1.5]
	static constexpr int FPGA_OUT_VALUE = GPIO_NUM_34;//FPGA PIN 69
	static constexpr int FPGA_GO_LIVE = GPIO_NUM_25;//FPGA PIN 58
	static constexpr int FPGA_TO_OUTPUT = GPIO_NUM_26;//FPGA PIN 54[~]
	static constexpr int FPGA_IN_VALUE = GPIO_NUM_27;//FPGA PIN 52
	static constexpr int FPGA_IN_SHIFT = GPIO_NUM_14;//FPGA PIN 50
	static constexpr int FPGA_EMERGENCY_STOP = GPIO_NUM_22;//FPGA PIN 71
};