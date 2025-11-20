#include "Aborter.hpp"
#include "../Storage/Flash.hpp"
#include "esp_attr.h"
LastAbortMessage* Aborter::getLastAbortReason(
	CleanupBucket& cleanupBucket){
	if(!Flash::getIsInitialized()){
		safeAbort(TAG, "Flash was not initialized when calling getLastAbortReason");
		return nullptr;
	}
	char* reason = nullptr;
	Flash::getCharStringOnHeap(TAG, REASON_KEY, reason, cleanupBucket);
	uint32_t* backtrace = nullptr;
	size_t backtraceLength = 0;
	Flash::getArray(TAG, BACKTRACE_KEY, backtrace, backtraceLength, 
							 cleanupBucket);
	if((!reason)&&((!backtrace)||backtraceLength<1)){
		return nullptr;
	}
	LastAbortMessage* lastAbortMessage = new LastAbortMessage(
		backtrace, backtraceLength, reason, SubsystemIdentifier::get());
	cleanupBucket.addDelete(lastAbortMessage);
	return lastAbortMessage;
}
bool Aborter::hasLastAbortReason(){
	if(!Flash::getIsInitialized()){
		safeAbort(TAG, "Flash was not initialized when calling getLastAbortReason");
		return true;
	}
	return Flash::hasKey(TAG, REASON_KEY)
		||Flash::hasKey(TAG, BACKTRACE_KEY);
}
void Aborter::clearLastAbortReason(){
	if(!Flash::getIsInitialized()){
		Log::Warn(TAG, "Flash was not initialized when calling clearLastAbortReason");
		return;
	}
	Flash::erase(TAG, REASON_KEY);
	Flash::erase(TAG, BACKTRACE_KEY);
}