#pragma once
#include "esp_heap_caps.h"
class MemoryHelper{
public:
    static size_t getAvailableHeap();
	static void printAvailableHeap();
};