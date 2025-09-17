#ifndef JHELPER_HPP
#define JHELPER_HPP

#include <string>
#include <optional>
#include <cstdint>
#include "../cJSON/cJSON.h"

class JHelper {
public:

	static std::shared_ptr<cJSON> toSharedPtr(cJSON* raw);
    // --- Non-nullable accessors (with success flag) ---
	static cJSON* getObject(cJSON* obj, const char* key, bool& success);
    static char* getString(cJSON* obj, const char* key, bool& success);
    static int8_t getInt8(cJSON* obj, const char* key, bool& success);
    static uint8_t getUInt8(cJSON* obj, const char* key, bool& success);
	static int16_t getInt16(cJSON* obj, const char* key, bool& success);
    static uint16_t getUInt16(cJSON* obj, const char* key, bool& success);
    static int32_t getInt32(cJSON* obj, const char* key, bool& success);
    static uint32_t getUInt32(cJSON* obj, const char* key, bool& success);
    static int64_t getInt64(cJSON* obj, const char* key, bool& success);
	static uint64_t getUInt64(cJSON* obj, const char* key, bool& success);
    static double getDouble(cJSON* obj, const char* key, bool& success);
    static bool getBool(cJSON* obj, const char* key, bool& success);

    // --- Nullable accessors using std::optional ---
	static cJSON* getNullableObject(
		cJSON* obj, const char* key, bool& success);
    static char* getNullableString(
		cJSON* obj, const char* key, bool& success);
    static std::optional<int8_t> getNullableInt8(
		cJSON* obj, const char* key, bool& success);
    static std::optional<uint8_t> getNullableUInt8(
		cJSON* obj, const char* key, bool& success);
    static std::optional<int16_t> getNullableInt16(
		cJSON* obj, const char* key, bool& success);
    static std::optional<uint16_t> getNullableUInt16(
		cJSON* obj, const char* key, bool& success);
    static std::optional<int32_t> getNullableInt32(
		cJSON* obj, const char* key, bool& success);
    static std::optional<uint32_t> getNullableUInt32(
		cJSON* obj, const char* key, bool& success);
    static std::optional<int64_t> getNullableInt64(
		cJSON* obj, const char* key, bool& success);
    static std::optional<uint64_t> getNullableUInt64(
		cJSON* obj, const char* key, bool& success);
    static std::optional<double> getNullableDouble(
		cJSON* obj, const char* key, bool& success);
    static std::optional<bool> getNullableBool(
		cJSON* obj, const char* key, bool& success);
		
		
		
		
	static void addObject(cJSON* obj, const char* key, cJSON* value);
    static void addString(cJSON* obj, const char* key, const char* value);
    static void addInt8(cJSON* obj, const char* key, int8_t value);
    static void addUInt8(cJSON* obj, const char* key, uint8_t value);
    static void addInt16(cJSON* obj, const char* key, int16_t value);
    static void addUInt16(cJSON* obj, const char* key, uint16_t value);
    static void addInt32(cJSON* obj, const char* key, int32_t value);
    static void addUInt32(cJSON* obj, const char* key, uint32_t value);
    static void addInt64(cJSON* obj, const char* key, int64_t value);
	static void addUInt64(cJSON* obj, const char* key, uint64_t value);
	static void setUInt64(cJSON* obj, const char* key, uint64_t value);
    static void addDouble(cJSON* obj, const char* key, double value);
    static void addBool(cJSON* obj, const char* key, bool value);
	
	
	
	static void addNullableObject(cJSON* obj, const char* key, cJSON* value);
    static void addNullableString(cJSON* obj, const char* key, char* value);
    static void addNullableInt8(cJSON* obj, const char* key, std::optional<int8_t> value);
    static void addNullableUInt8(cJSON* obj, const char* key, std::optional<uint8_t> value);
    static void addNullableInt16(cJSON* obj, const char* key, std::optional<int16_t> value);
    static void addNullableUInt16(cJSON* obj, const char* key, std::optional<uint16_t> value);
    static void addNullableInt32(cJSON* obj, const char* key, std::optional<int32_t> value);
    static void addNullableUInt32(cJSON* obj, const char* key, std::optional<uint32_t> value);
    static void addNullableInt64(cJSON* obj, const char* key, std::optional<int64_t> value);
    static void addNullableUInt64(cJSON* obj, const char* key, std::optional<uint64_t> value);
    static void addNullableDouble(cJSON* obj, const char* key, std::optional<double> value);
    static void addNullableBool(cJSON* obj, const char* key, std::optional<bool> value);
	static void printJsonKeysAndValues(cJSON* json);
};

#endif // JHELPER_HPP
