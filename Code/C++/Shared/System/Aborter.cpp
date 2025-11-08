#include "Aborter.hpp"
#include "esp_attr.h"
#include <cstring>
IRAM_ATTR char Aborter::_reasonBuffer[128] = {0};
IRAM_ATTR bool Aborter::_hasReason = false;
const char* Aborter::getLastAbortReason(){
    return _hasReason?_reasonBuffer:nullptr;
}
void Aborter::clearLastAbortReason(){
    std::memset(_reasonBuffer, 0, sizeof(_reasonBuffer));
	_hasReason = false;
}