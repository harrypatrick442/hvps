#include "Aborter.hpp"
#include "../Storage/Flash.hpp"
#include "../Timing/Delay.hpp"
#include "SubsystemIdentifier.hpp"
#include "esp_attr.h"
#include "esp_system.h"
const char* Aborter::TAG = "Aborter";
[[noreturn]] void Aborter::_safeAbort(const char* fileName, char* formatted){
    LOG_FATAL(fileName, formatted);
	
    constexpr int BACKTRACE_DEPTH = 16;
    uint32_t backtrace[BACKTRACE_DEPTH] = {0};
    size_t backtraceLength = BacktraceHelper::getBacktrace(
		backtrace, nullptr, BACKTRACE_DEPTH);
		
	if(Flash::getIsInitialized()){
		Flash::setString(TAG, REASON_KEY,
			formatted);
		Flash::setArray(TAG, BACKTRACE_KEY,
			backtrace, backtraceLength);
	}
	else{
		LOG_WARN("Flash was not initialized when trying to set last abort reason");
	}
	
	Delay::ms(200);

    esp_restart();

    while (true) { }
}
LastAbortMessage* Aborter::getLastAbortReason(
	CleanupBucket& cleanupBucket){
	if(!Flash::getIsInitialized()){
		SAFE_ABORT("Flash was not initialized when calling getLastAbortReason");
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
		SAFE_ABORT("Flash was not initialized when calling getLastAbortReason");
		return true;
	}
	return Flash::hasKey(TAG, REASON_KEY)
		||Flash::hasKey(TAG, BACKTRACE_KEY);
}
void Aborter::clearLastAbortReason(){
	if(!Flash::getIsInitialized()){
		LOG_WARN("Flash was not initialized when calling clearLastAbortReason");
		return;
	}
	Flash::erase(TAG, REASON_KEY);
	Flash::erase(TAG, BACKTRACE_KEY);
}