#pragma once
#include "esp_system.h"
#include "esp_log.h"
#include "esp_core_dump.h"
#include "../Logging/Log.hpp"
#include "../Core/CleanupBucket.hpp"
#include "StackSamplerHelper.hpp"
#include <cstring>
#include "Generated/Messages/CoreDumpSummaryMessage.hpp"
#include "SubsystemIdentifier.hpp"


//YOU MUST ENABLE DUMPING TO FLASH FOR THIS TO WORK. MR WOBOT LOVES DUMPING AND FLASHING!🤖<(grrr)
class Aborter;

class CrashReporter {
public:

	static inline bool hasCoreDumpSummary() {
		esp_core_dump_summary_t summary;
		if (esp_core_dump_get_summary(&summary) != ESP_OK) {
			return false;
		}
		return true;
	}
	static inline CoreDumpSummaryMessage* getCoreDumpSummary(CleanupBucket& cleanupBucket) {
		esp_core_dump_summary_t* summary = new esp_core_dump_summary_t();
		cleanupBucket.addDelete(summary);
		if (esp_core_dump_get_summary(summary) != ESP_OK) {
			LOG_INFO("No valid core dump summary (or not ELF-to-flash).");
			return nullptr;
		}
		//printUsefulSummaryInfo(summary);
		esp_core_dump_bt_info_t& backtrace_info = summary->exc_bt_info;
		esp_core_dump_summary_extra_info_t& extra_info = summary->ex_info;
		const char* crashingApplicationsSHA256SumAsAString = convertSha256ToHexCstr(summary->app_elf_sha256, cleanupBucket);
		
		char* taskName = new char[17];
		cleanupBucket.addDeleteArray(taskName);
		memcpy(taskName, summary->exc_task, 16); // copy exactly 16 bytes
		taskName[16] = '\0'; 
		
		auto coreDumpSummaryMessage =  new CoreDumpSummaryMessage(
			   extra_info.exc_a/*aRegisterSetWhenTheExceptionCaused*/,
			   16/*aRegisterSetWhenTheExceptionCausedLength*/,
			   backtrace_info.bt/*backtrace*/,
			   backtrace_info.depth/*backtraceLength*/,
			   backtrace_info.corrupted/*!< Status flag for backtrace is corrupt or not */,
			   extra_info.epcx_reg_bits/*bitMaskOfAvailableEPCxRegisters*/,
			   extra_info.exc_cause/*causeOfException*/,
			   crashingApplicationsSHA256SumAsAString, 
			   extra_info.epcx/*pCRegisterAddressAtExceptionLevel1To7*/,
			   EPCx_REGISTER_COUNT/*pCRegisterAddressAtExceptionLevel1To7Length*/,
			   summary->exc_pc/*programCounterForException*/, 
			   SubsystemIdentifier::get(),
			   taskName,
			   summary->exc_tcb/*taskPointer*/,
			   summary->core_dump_version, 
			   extra_info.exc_vaddr/*virtualAddressOfException*/
		);
		cleanupBucket.addDelete(coreDumpSummaryMessage);
		return coreDumpSummaryMessage;
	}
	static inline const char* convertSha256ToHexCstr(const uint8_t* hash, CleanupBucket& cleanupBucket) {
		// 2 chars per byte + 1 null terminator
		size_t outputLength = APP_ELF_SHA256_SZ * 2 + 1;
		char* result = new char[outputLength];
		cleanupBucket.addDeleteArray(result);
		const char* hexDigits = "0123456789abcdef";
		
		for (size_t i = 0; i < APP_ELF_SHA256_SZ; ++i) {
			result[i * 2]     = hexDigits[(hash[i] >> 4) & 0xF]; // high nibble
			result[i * 2 + 1] = hexDigits[hash[i] & 0xF];        // low nibble
		}

		result[outputLength - 1] = '\0'; // null terminator
		return result;
	}
	static inline void printUsefulSummaryInfo(esp_core_dump_summary_t* summary){
		esp_core_dump_bt_info_t& backtrace_info = summary->exc_bt_info;
		LOG_INFO("Crash task: %s  PC=0x%08" PRIx32, summary->exc_task, summary->exc_pc);
		LOG_INFO("Backtrace depth=%u  corrupted=%u", (unsigned)backtrace_info.depth, (unsigned)backtrace_info.corrupted);
		for (uint32_t i = 0; i < backtrace_info.depth; ++i) {
			LOG_INFO("  #%u 0x%08" PRIxPTR, (unsigned)i, (uintptr_t)backtrace_info.bt[i]);
		}
		for (uint32_t i = 0; i < 16; ++i) {
			LOG_INFO("A[%u] = 0x%08" PRIx32, i, summary->ex_info.exc_a[i]);
		}
	}
/*
	static inline bool getRecord(CrashRecord& crashRecord) {
		esp_err_t err = esp_core_dump_image_check();
		if(err==ESP_ERR_NOT_FOUND) {
			return false;
		}
		if(err==ESP_ERR_INVALID_SIZE){
			LOG_WARN("Core dump was present but had invalid size!");
			return false;
		}
		if(err==ESP_ERR_INVALID_CRC){
			LOG_WARN("Core dump but was corrupted!");
			return false;
		}

		size_t dumpAddr = 0;
		size_t dumpLen  = 0;
		err = esp_core_dump_image_get(&dumpAddr, &dumpLen);
		crashRecord.reason = esp_reset_reason();

		if (err != ESP_OK) {
			const char* errName = esp_err_to_name(err);
			char buf[128];
			snprintf(buf, sizeof(buf),
					 "Failed to read core dump (err=%s).", errName);
			crashRecord.message = buf;
			return true;
		}

		if (dumpLen == 0) {
			crashRecord.message = "Failed to read core dump (dumpLen=0).";
			return true;
		}

		const void* mapped = nullptr;
		spi_flash_mmap_handle_t handle;
		err = spi_flash_mmap(dumpAddr, dumpLen,
							 SPI_FLASH_MMAP_DATA,
							 &mapped, &handle);
		if (err != ESP_OK) {
			crashRecord.message =
				std::string("spi_flash_mmap failed: ") + esp_err_to_name(err);
			return true;
		}

		crashRecord.dump.assign(
			reinterpret_cast<const uint8_t*>(mapped),
			reinterpret_cast<const uint8_t*>(mapped) + dumpLen);

		spi_flash_munmap(handle);

		crashRecord.message = "Core dump retrieved.";
		//LOG_INFO("Core dump length: %lu bytes", (unsigned long)dumpLen);

		return true;
	}*/
    static inline void clearRecord() {
        esp_core_dump_image_erase();
    }
	static inline void causePanicOnPurpose() {
		ESP_ERROR_CHECK(ESP_FAIL);  // triggers panic handler
		// or:
		//abort();                     // triggers core dump too
	}
};
