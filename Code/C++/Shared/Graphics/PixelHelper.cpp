#include "PixelHelper.hpp"
void PixelHelper::uInt32ToRGB(uint32_t pixel, uint8_t& r, uint8_t& g, uint8_t& b)
{
	r = static_cast<uint8_t>(pixel & 0xFF);
	g = static_cast<uint8_t>((pixel >> 8) & 0xFF);
	b = static_cast<uint8_t>((pixel >> 16) & 0xFF);
}
uint32_t PixelHelper::rGBToUInt32(uint8_t r, uint8_t g, uint8_t b)
{
	return static_cast<uint32_t>(r) |
		   (static_cast<uint32_t>(g) << 8) |
		   (static_cast<uint32_t>(b) << 16);
}