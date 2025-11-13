#include "Aborter.hpp"
#include "../Storage/Flash.hpp"
#include "esp_attr.h"
bool Aborter::getLastAbortReason(std::string& reason){
	if(!Flash::getIsInitialized()){
		Log::Warn(TAG, "Flash was not initialized when calling getLastAbortReason");
		return false;
	}
	return Flash::getString(TAG, REASON_KEY,
			reason);
}
void Aborter::clearLastAbortReason(){
	if(!Flash::getIsInitialized()){
		Log::Warn(TAG, "Flash was not initialized when calling clearLastAbortReason");
		return;
	}
	Flash::erase(TAG, REASON_KEY);
}