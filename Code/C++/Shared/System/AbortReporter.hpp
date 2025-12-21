#pragma once
#include "esp_system.h"
#include "esp_log.h"
#include "esp_core_dump.h"
#include "CrashRecord.h"
#include "../Logging/Log.hpp"
#include "StackSamplerHelper.hpp"
#include <cstring>


//YOU MUST ENABLE DUMPING TO FLASH FOR THIS TO WORK. MR WOBOT LOVES DUMPING AND FLASHING!🤖<(grrr)
//TODO what about Outputs::toSafe on different types of crash...
class Aborter;

class CrashReporter {
private:
    // Lives in RTC slow memory; survives soft resets
    static RTC_NOINIT_ATTR volatile CrashRecord _crashRecord;
    static inline constexpr uint32_t CRASH_MAGIC = 0xC0DEBEEF;
    static inline constexpr size_t N_WORDS_FROM_STACK = 8;
    static inline bool inAbort = false;

public:
    //-----------------------------------------------------------------
    // initialize() — called once on startup
    //-----------------------------------------------------------------
    static inline void initialize() {
        static bool initialized = false;
        if (initialized) {
            SAFE_ABORT("CrashReporter already initialized!");
        }

        if (_crashRecord.magic != CRASH_MAGIC) {
            clearCrashRecordStruct();
        }

        // Check flash for an existing core dump and cache summary info
        refreshFromCoreDump();

        initialized = true;
    }
	static inline void refreshFromCoreDump()
	{
		clearCrashRecordStruct();

		// Check if a valid core dump image exists
		if (!esp_core_dump_image_check()) {
			_crashRecord.message = "No core dump found in flash.";
			return;
		}

		_crashRecord.magic  = CRASH_MAGIC;
		const void* dumpData = nullptr;
		uint32_t dumpLen = 0;
		esp_err_t err = esp_core_dump_image_get(&dumpData, &dumpLen);

		if (err != ESP_OK || dumpLen == 0) {
			char buf[128];
			snprintf(buf, sizeof(buf),
					 "Failed to read core dump from flash (err=%d).", (int)err);
			_crashRecord.message = buf;
			LOG_ERROR("%s", buf);
			return;
		}

		// Convert entire dump to string (binary-safe)
		_crashRecord.message.assign(
			reinterpret_cast<const char*>(dumpData),
			reinterpret_cast<const char*>(dumpData) + dumpLen
		);
		LOG_INFO("Read %lu-byte core dump from flash.", (unsigned long)dumpLen);

		// Optional: erase the dump once stored
		// esp_core_dump_image_erase();
	}


    //-----------------------------------------------------------------
    // Accessors
    //-----------------------------------------------------------------
    static inline bool getRecord(CrashRecord& rec) {
        if (_crashRecord.magic != CRASH_MAGIC) return false;
        rec = _crashRecord;
        return true;
    }

    static inline void clearRecord() {
        esp_core_dump_image_erase();
        clearCrashRecordStruct();
    }

    static inline void printRecord() {
        CrashRecord rec;
        if (!getRecord(rec)) {
            LOG_INFO("There was no previous crash record.");
            return;
        }

        LOG_WARN("Previous crash detected:");
        LOG_WARN("Reason: %d", rec.reason);
        LOG_WARN("Core dump size: %lu bytes", rec.coreDumpSize);
        LOG_WARN("Read err: %d", rec.coreDumpReadErr);
        LOG_WARN("Message: %s", rec.message);
    }
/*
    //-----------------------------------------------------------------
    // onPanic() — fallback for manual panic trapping
    //-----------------------------------------------------------------
    static inline void IRAM_ATTR onPanic(void* frame, int core_id, bool is_abort) {
        _crashRecord.magic  = CRASH_MAGIC;
        _crashRecord.reason = ESP_RST_PANIC;
        StackSamplerHelper::storePcSpExec(_crashRecord.pc, _crashRecord.sp, _crashRecord.excCause);
        StackSamplerHelper::sampleStackWords(_crashRecord.sp, _crashRecord.trace, N_WORDS_FROM_STACK);
        copyConstMsg(_crashRecord.message, sizeof(_crashRecord.message),
                     "PANIC: CPU exception or watchdog");
        esp_restart_noos();
        while (true) {}
    }
*/
	static inline void IRAM_ATTR abort()
	{
		if (inAbort) {
			esp_restart();
		}
		inAbort = true;

		clearCrashRecordStruct();

		_crashRecord.magic  = CRASH_MAGIC;
		_crashRecord.reason = esp_reset_reason();

		uint32_t pc = 0, sp = 0, excCause = 0;
		uint32_t trace[8] = {0};

		// Capture CPU state
		StackSamplerHelper::storePcSpExec(pc, sp, excCause);
		StackSamplerHelper::sampleStackWords(sp, trace, 8);

		// Build formatted message
		char buf[512];
		int len = snprintf(buf, sizeof(buf),
			"Abort called\n"
			"Reset reason: %u\n"
			"Program counter (PC): 0x%08X\n"
			"Stack pointer (SP): 0x%08X\n"
			"Trace: 0x%08X, 0x%08X, 0x%08X, 0x%08X, "
			"0x%08X, 0x%08X, 0x%08X, 0x%08X\n",
			(unsigned)_crashRecord.reason,
			(unsigned)pc, (unsigned)sp,
			trace[0], trace[1], trace[2], trace[3],
			trace[4], trace[5], trace[6], trace[7]
		);

		// Assign to string (truncates if necessary)
		_crashRecord.message.assign(buf, len);

		// Optional: append exception cause if nonzero
		if (excCause) {
			char excBuf[64];
			snprintf(excBuf, sizeof(excBuf), "Exception cause: 0x%08X\n", (unsigned)excCause);
			_crashRecord.message += excBuf;
		}

		esp_restart();
	}

private:

    static inline void IRAM_ATTR clearCrashRecordStruct() {
		_crashRecord.message.clear();
		_crashRecord.magic  = CRASH_MAGIC;
		_crashRecord.reason = 0;
		_crashRecord.errorCode = 0;
    }
};
