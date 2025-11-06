#pragma once
#include <vector>
#include "Core/CleanupBucket.hpp"
#include <cstdint>
class Base64
{
public:
	static size_t getEncodeLength(const std::vector<uint8_t>& data);
	static char* encode(const std::vector<uint8_t>& data, CleanupBucket& cleanupBucket);
	static void encodeToCharArray(
		size_t indexFromInclusive,
		char* charArray,
		const std::vector<uint8_t>& data);
};