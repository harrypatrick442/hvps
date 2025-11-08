#include "CrashRecord.hpp"
#include "Encoding/Base64.hpp"
#include "Core/CleanupBucket.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>  
const char* CrashRecord::toJSONString(
	CleanupBucket& cleanupBucket, 
	size_t& outputLength
) const{
	size_t startSubstringLength;
	CleanupBucket internalCleanupBucket;
	const char* startSubstring = buildShortSubstring(
		"{\"reason\":%d,\"message\":\"%s\",\"dumpB64\":\"", 
		internalCleanupBucket,
		startSubstringLength,
		static_cast<int>(reason),
		"some message"
	);
	const char* endSubstring = "\"}";
	size_t endSubstringLength = strlen(endSubstring);
	size_t encodedLength = Base64::getEncodeLength(dump);
	outputLength = encodedLength
		+startSubstringLength
		+endSubstringLength;
	char* output = new char[outputLength+1];
	if (!output) return nullptr;
	cleanupBucket.addDeleteArray(output);
	std::memcpy(
		output, 
		startSubstring,
		startSubstringLength
	);
	std::memcpy(
		output+startSubstringLength+encodedLength,
		endSubstring, 
		endSubstringLength
	);
	Base64::encodeToCharArray(
		startSubstringLength,
		output,
		dump);
	output[outputLength]='\0';
	return output;
}
const char* CrashRecord::buildShortSubstring(const char* format, CleanupBucket& bucket, size_t& outLength, ...) const {
    va_list args;

    // First, do a dry run to get the size needed
    va_start(args, outLength);
    int required = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (required < 0) {
        outLength = 0;
        return nullptr;
    }

    outLength = static_cast<size_t>(required);
    char* buffer = new char[outLength + 1];  // +1 for null terminator
    bucket.addDeleteArray(buffer);

    va_start(args, outLength);
    vsnprintf(buffer, outLength + 1, format, args);
    va_end(args);

    return buffer;
}