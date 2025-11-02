#include "CrashReporter.hpp"
// Volatile because it's stored in RTC slow memory and must not be optimized away.
RTC_NOINIT_ATTR volatile CrashRecord CrashReporter::_crashRecord{};  // definition in RTC RAM!

//----------------------------------------------------
// Panic handler: minimal operations only
//----------------------------------------------------
extern "C" void IRAM_ATTR my_panic_handler(void* frame, int core_id, bool is_abort)
{
    CrashReporter::onPanic(frame, core_id, is_abort);
}

//----------------------------------------------------
// abort() override — catches any standard abort
//----------------------------------------------------
extern "C" void abort(void)
{
	CrashReporter::abort();
}