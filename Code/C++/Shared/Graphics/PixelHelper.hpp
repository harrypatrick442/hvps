#pragma once
#include <stdint.h>
class PixelHelper{
public:
	static void uInt32ToRGB(uint32_t pixel, uint8_t& r, uint8_t& g, uint8_t& b);
	static uint32_t rGBToUInt32(uint8_t r, uint8_t g, uint8_t b);
};