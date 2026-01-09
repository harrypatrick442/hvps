#include "MemoryHelper.hpp"
#include "Logging/Log.hpp"
size_t MemoryHelper::getAvailableHeap() {
	return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}
void MemoryHelper::printAvailableHeap(){
	size_t available = getAvailableHeap();
	LOG_INFO("Available heap (8-bit capable): %u bytes", available);
}