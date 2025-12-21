#include "DisplayBuffer.hpp"
#include "System/SafeAbort.hpp"
#include "esp_heap_caps.h"
DisplayBuffer::DisplayBuffer(size_t length):
	_length(length),
	_nextIndex(0){
	if(length<1){
		SAFE_ABORT("length must be greater than zero");
	}
	_pixels = (volatile uint32_t*) heap_caps_malloc(
		length * sizeof(uint32_t),
		MALLOC_CAP_DMA  // or MALLOC_CAP_INTERNAL if you want it strictly in internal RAM
	);
}
volatile uint32_t* DisplayBuffer::takePixels(size_t& length){
	if(_nextIndex>=_length){
		SAFE_ABORT("Buffer was not long enough");
	}
	volatile uint32_t* pointer = _pixels+_nextIndex;
	_nextIndex+=length;
	return pointer;
}