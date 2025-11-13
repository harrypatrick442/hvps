#include "Aborter.hpp"
#include "../Storage/Flash.hpp"
#include "esp_attr.h"
LastAbortMessage* Aborter::getLastAbortReason(
	CleanupBucket& cleanupBucket){
	if(!Flash::getIsInitialized()){
		Log::Warn(TAG, "Flash was not initialized when calling getLastAbortReason");
		return nullptr;
	}
	char* reason = nullptr;
	Flash::getCharStringOnHeap(TAG, REASON_KEY, reason, cleanupBucket);
	uint32_t* backtrace = nullptr;
	size_t backtraceLength = 0;
	Flash::getArray(TAG, BACKTRACE_KEY, backtrace, backtraceLength, 
							 cleanupBucket);
	LastAbortMessage* lastAbortMessage = new LastAbortMessage(
		backtrace, backtraceLength, reason);
	cleanupBucket.addDelete(lastAbortMessage);
	return lastAbortMessage;
}
void Aborter::clearLastAbortReason(){
	if(!Flash::getIsInitialized()){
		Log::Warn(TAG, "Flash was not initialized when calling clearLastAbortReason");
		return;
	}
	Flash::erase(TAG, REASON_KEY);
}