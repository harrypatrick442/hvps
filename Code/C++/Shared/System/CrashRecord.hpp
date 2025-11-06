#pragma once
#include "esp_system.h"
#include <vector>
#include <string>
class CleanupBucket; 
struct CrashRecord {
    esp_reset_reason_t reason;     // Reset cause
    std::vector<uint8_t> dump;     // Raw core dump bytes
    std::string message;           // Description or error message
	const char* toJSONString(
		CleanupBucket& cleanupBucket, 
		size_t& outputLength
	) const;
	private:
		const char* buildShortSubstring(const char* format, CleanupBucket& bucket, size_t& outLength, ...) const;
};