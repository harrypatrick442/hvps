#pragma once
#include "esp_system.h"
#include "esp_log.h"
#include "CrashRecord.h"
#include "../Logging/Log.hpp"
#include "StackSamplerHelper.hpp"
#include <cstring>
#include "esp_core_dump.h"
#include "esp_private/panic_internal.h"
#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//Remove unneeded dependencies

class Aborter;
extern "C" void IRAM_ATTR my_panic_handler(void* frame, int core_id, bool is_abort);
extern "C" void abort(void);

//----------------------------------------------------
// CrashReporter — now using RTC slow memory only
// Survives software resets and panics, but cleared on power loss
//----------------------------------------------------
class CrashReporter {
private:
    // Lives in RTC slow memory, survives soft resets
    static volatile CrashRecord _crashRecord;
    static inline constexpr uint32_t CRASH_MAGIC  = 0xC0DEBEEF;
    static inline constexpr size_t N_WORDS_FROM_STACK  = 8;
    static inline bool inAbort = false;

public:

    static inline void initialize() {
        static bool initialized = false;   // persists across calls
        if (initialized) {
            // already initialized — this is a fatal logic error
            SAFE_ABORT("CrashReporter already initialized!");
        }
		
		//Ensures garbage in RTC RAM after brownouts doesn’t get misread as a valid crash.
		if (_crashRecord.magic != CRASH_MAGIC)
		{
			clearCrashRecordStruct();
		}
		
        //Before you cheeky gits assume this was all AI generated. We did it together lol.
        //Part him part me until it was perfect.
        esp_set_panic_handler(my_panic_handler);
        initialized = true;
    }

    //----------------------------------------------------
    // Save last known software error (safe anywhere)
    //----------------------------------------------------
    static inline void __attribute__((always_inline)) IRAM_ATTR saveLastErrorMessage(const char* fmt, ...)
    {
		clearCrashRecordStruct();
        va_list args;
        va_start(args, fmt);
        vsnprintf(_crashRecord.message, sizeof(_crashRecord.message), fmt, args);
        va_end(args);
		_crashRecord.magic  = CRASH_MAGIC;
		_crashRecord.reason = esp_reset_reason();
		StackSamplerHelper::storePcSpExec(_crashRecord.pc, _crashRecord.sp, _crashRecord.excCause);
		StackSamplerHelper::sampleStackWords(_crashRecord.sp, _crashRecord.trace, N_WORDS_FROM_STACK);
    }

    //----------------------------------------------------
    // Retrieve previous crash record (after soft reset)
    //----------------------------------------------------
    static inline bool getRecord(CrashRecord& rec) {
        if (_crashRecord.magic != CRASH_MAGIC)
            return false;

        rec = _crashRecord;
        return true;
    }

    //----------------------------------------------------
    // Clear previous crash record (mark as processed)
    //----------------------------------------------------
    static inline void clearRecord() {
		clearCrashRecordStruct();
        _crashRecord.magic = 0;//Not really needed but making it clear that it's doing this.
        return true;
    }

    //----------------------------------------------------
    // Print previous crash (if any)
    //----------------------------------------------------
    static inline void printRecord() {
        CrashRecord rec;
        if (!getRecord(rec)) {
            LOG_INFO("There was no previous crash record :)");
            return;
        }
        LOG_WARN("Previous crash detected:");
        LOG_WARN("Reason: %d", rec.reason);
        LOG_WARN("Message: %s", rec.message);
    }

    //----------------------------------------------------
    // Called by ESP-IDF panic handler
    //----------------------------------------------------
	static inline __attribute__((always_inline)) void IRAM_ATTR onPanic(void* /*frame*/, int /*core_id*/, bool /*is_abort*/)
	{
		/*clearCrashRecordStruct(); Do not clear here as tight watchdog budget. Clearing is more about
		safeguarding crossover values from multiple errors if future additions are made and we forget to overwrite them
		*/
		_crashRecord.magic  = CRASH_MAGIC;
		_crashRecord.reason = ESP_RST_PANIC;

		// PC/SP/EXC (portable across Xtensa/RISC-V)
		StackSamplerHelper::storePcSpExec(_crashRecord.pc, _crashRecord.sp, _crashRecord.excCause);

		// Grab up to N_WORDS_FROM_STACK words from the current stack (addresses to decode later)
		StackSamplerHelper::sampleStackWords(_crashRecord.sp, _crashRecord.trace, N_WORDS_FROM_STACK);

		// Message without libc
		copyConstMsg(_crashRecord.message, sizeof(_crashRecord.message),
					   "PANIC: CPU exception or watchdog");

		// No OS services here; use the no-OS restart path
		esp_restart_noos();
		while (true) { } // Should not return; belt-and-braces
	}

    //----------------------------------------------------
    // Safe abort for controlled shutdowns
    //----------------------------------------------------
    static inline void IRAM_ATTR abort() {        
        if (inAbort)
            esp_restart();

        inAbort = true;
        saveLastErrorMessage("Abort called");
        esp_restart();
    }
private:
	static inline __attribute__((always_inline)) void IRAM_ATTR copyConstMsg(char* dst, size_t dst_sz, const char* src) {
		if (dst_sz == 0) return;
		size_t i = 0;
		for (; src[i] && i + 1 < dst_sz; ++i) dst[i] = src[i];
		dst[i++] = '\0';
	}
    static inline __attribute__((always_inline)) void IRAM_ATTR clearCrashRecordStruct() {
        // Zero entire record, ensuring message and trace are clean
        uint8_t* p = reinterpret_cast<uint8_t*>(&_crashRecord);
        for (size_t i = 0; i < sizeof(_crashRecord); ++i)
            p[i] = 0;
    }
};
