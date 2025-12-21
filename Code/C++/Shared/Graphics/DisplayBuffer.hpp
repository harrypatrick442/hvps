#pragma once
#include <stdint.h>
#include <cstddef>
class DisplayBuffer{
private:
	size_t _length;
	size_t _nextIndex;
	volatile uint32_t* _pixels;
public:
	DisplayBuffer(size_t length);
	volatile uint32_t* takePixels(size_t& length);
};