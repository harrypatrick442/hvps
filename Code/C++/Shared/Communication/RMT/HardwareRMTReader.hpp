#pragma once
class HardwareRMTReader{
private:
	uint8_t _currentByte = 0;
	uint8_t _nextNBit = 0;
	RingbufHandle_t _ringBufferHandle;

}