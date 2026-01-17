#include "Aborter.hpp"
#include "../Storage/Flash.hpp"
#include "../Timing/Delay.hpp"
#include "Logging/Log.hpp"
#include "SubsystemIdentifier.hpp"
#include "esp_attr.h"
#include "esp_system.h"
const char* Aborter::REASON_KEY = "reason";
const char* Aborter::BACKTRACE_KEY = "bt";
const char* Aborter::TAG = "Aborter";
std::function<void()> Aborter::_toSafe = nullptr;
std::terminate_handler Aborter::_oldTerminateHandler = nullptr;

void Aborter::initialize(const std::function<void()>& toSafe) {
    // Install once; chain the old handler
    _oldTerminateHandler = std::set_terminate(&Aborter::terminateHandler);
    _toSafe = toSafe;
}

[[noreturn]] void Aborter::terminateHandler() noexcept {
    // 1) Put hardware in a safe state ASAP
    if (_toSafe) {
        _toSafe();
    }

    // 2) Chain to previous handler if it exists
    if (_oldTerminateHandler) {
        _oldTerminateHandler(); // must be noreturn
    }

    // 3) Absolute fallback
    std::abort();
}

[[noreturn]] void Aborter::_safeAbort(const char* fileName, int lineNumber, char* message){
	char formatted[320];
	std::snprintf(formatted, sizeof(formatted), "line %d: %s", lineNumber, message);
	_safeAbort(fileName, formatted);
}
[[noreturn]] void Aborter::_safeAbort(const char* fileName, char* formatted){
    Log::Fatal(fileName, formatted);
	
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