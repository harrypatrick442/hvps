#pragma once
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_core_dump.h"
#include "CrashRecord.h"
#include "../Logging/Log.hpp"
#include "StackSamplerHelper.hpp"
#include <cstring>


//YOU MUST ENABLE DUMPING TO FLASH FOR THIS TO WORK. MR WOBOT LOVES DUMPING AND FLASHING!🤖<(grrr)
class Aborter;

class CrashReporter {
private:

    static inline constexpr const char* TAG = "CrashReporter";

public:
    static inline void initialize() {
        static bool initialized = false;
        if (initialized) {
            Aborter::safeAbort(TAG, "CrashReporter already initialized!");
        }
        initialized = true;
    }
	static inline bool getRecord(CrashRecord& crashRecord) {
		esp_err_t err = esp_core_dump_image_check();
		if(err==ESP_ERR_NOT_FOUND) {
			return false;
		}
		if(err==ESP_ERR_INVALID_SIZE){
			Log::Warn(TAG, "Core dump was present but had invalid size!");
			return false;
		}
		if(err==ESP_ERR_INVALID_CRC){
			Log::Warn(TAG, "Core dump but was corrupted!");
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
		//Log::Info(TAG, "Core dump length: %lu bytes", (unsigned long)dumpLen);

		return true;
	}
    static inline void clearRecord() {
        esp_core_dump_image_erase();
    }
	static inline void causePanicOnPurpose() {
		ESP_ERROR_CHECK(ESP_FAIL);  // triggers panic handler
		// or:
		//abort();                     // triggers core dump too
	}
};
