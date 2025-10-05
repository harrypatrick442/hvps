	#include "JHelper.hpp"
	#include <string.h>
	std::shared_ptr<cJSON> JHelper::toSharedPtr(cJSON* raw) {
		return std::shared_ptr<cJSON>(raw, cJSON_Delete); // custom deleter
	}
	cJSON* JHelper::getObject(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item!=nullptr){
			return item;
		}
		success = false;
		return nullptr;
	}/*
	cJSON* JHelper::getArray(
		cJSON* obj, const char* key, bool& success){
		CJSON* jArray = JHelper::getObject(obj, key, success);
		if(!success){
			return nullptr;
		}
		int size = cJSON_GetArraySize(jArray);
		for (int i = 0; i < size; i++) {
			cJSON *item = cJSON_GetArrayItem(jArray, i);
			if (cJSON_IsString(item)) {
				printf("Fruit %d: %s\n", i, item->valuestring);
			}
		}
	}*/
	char* JHelper::getString(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsString(item)) {
			return strdup(item->valuestring);;
		}
		success = false;
		return nullptr;
	}

	int8_t JHelper::getInt8(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<int8_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}
	uint8_t JHelper::getUInt8(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<uint8_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}

	int16_t JHelper::getInt16(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<int16_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}
	uint16_t JHelper::getUInt16(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<uint16_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}


	int32_t JHelper::getInt32(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<int32_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}
	uint32_t JHelper::getUInt32(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<uint32_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}

	int64_t JHelper::getInt64(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<int64_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}
	uint64_t JHelper::getUInt64(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return static_cast<uint64_t>(item->valuedouble);
		}
		success = false;
		return 0;
	}

	double JHelper::getDouble(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsNumber(item)) {
			return item->valuedouble;
		}
		success = false;
		return 0.0;
	}

	bool JHelper::getBool(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if (cJSON_IsBool(item)) {
			return cJSON_IsTrue(item);
		}
		success = false;
		return false;
	}

	// -------------------- Nullable Methods --------------------

	cJSON* JHelper::getNullableObject(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		return item;
	}
	char* JHelper::getNullableString(
		cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return nullptr;
		}
		if (cJSON_IsString(item)) {
			return strdup(item->valuestring);
		}
		success = false;
		return nullptr;
	}

	std::optional<int8_t> JHelper::getNullableInt8(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<int8_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}
	std::optional<uint8_t> JHelper::getNullableUInt8(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<uint8_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}

	std::optional<int16_t> JHelper::getNullableInt16(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<int16_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}
	std::optional<uint16_t> JHelper::getNullableUInt16(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<uint16_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}

	std::optional<int32_t> JHelper::getNullableInt32(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<int32_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}

	std::optional<uint32_t> JHelper::getNullableUInt32(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<uint32_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}
	std::optional<int64_t> JHelper::getNullableInt64(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<int64_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}
	std::optional<uint64_t> JHelper::getNullableUInt64(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return static_cast<uint64_t>(item->valuedouble);
		}
		success = false;
		return std::nullopt;
	}

	std::optional<double> JHelper::getNullableDouble(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsNumber(item)) {
			return item->valuedouble;
		}
		success = false;
		return std::nullopt;
	}

	std::optional<bool> JHelper::getNullableBool(
	cJSON* obj, const char* key, bool& success) {
		cJSON* item = cJSON_GetObjectItem(obj, key);
		if(item==nullptr){
			return std::nullopt;
		}
		if (cJSON_IsBool(item)) {
			return cJSON_IsTrue(item);
		}
		success = false;
		return std::nullopt;
	}
	
	
	
	
	
	
	void JHelper::addObject(cJSON* obj, const char* key, cJSON* value){
		cJSON_AddItemToObject(obj, key, value);
	}
    void JHelper::addString(cJSON* obj, const char* key, const char* value){
		cJSON_AddStringToObject(obj, key, value);
	}
    void JHelper::addInt8(cJSON* obj, const char* key, int8_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addUInt8(cJSON* obj, const char* key, uint8_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addUInt16(cJSON* obj, const char* key, uint16_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addUInt32(cJSON* obj, const char* key, uint32_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addInt32(cJSON* obj, const char* key, int32_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addInt64(cJSON* obj, const char* key, int64_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addUInt64(cJSON* obj, const char* key, uint64_t value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::setUInt64(cJSON* obj, const char* key, uint64_t value){
		cJSON_DeleteItemFromObjectCaseSensitive(obj, key);
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addDouble(cJSON* obj, const char* key, double value){
		cJSON_AddNumberToObject(obj, key, value);
	}
    void JHelper::addBool(cJSON* obj, const char* key, bool value){
		cJSON_AddBoolToObject(obj, key, value);
	}
	
	
	void JHelper::addNullableObject(cJSON* obj, const char* key, cJSON* value){
		if(value==nullptr)
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddItemToObject(obj, key, value);
	}
    void JHelper::addNullableString(cJSON* obj, const char* key, char* value){
		if(value==nullptr)
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddStringToObject(obj, key, value);
	}
    void JHelper::addNullableInt8(cJSON* obj, const char* key, std::optional<int8_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableUInt8(cJSON* obj, const char* key, std::optional<uint8_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableUInt16(cJSON* obj, const char* key, std::optional<uint16_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableUInt32(cJSON* obj, const char* key, std::optional<uint32_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableInt32(cJSON* obj, const char* key, std::optional<int32_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableInt64(cJSON* obj, const char* key, std::optional<int64_t> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableDouble(cJSON* obj, const char* key, std::optional<double> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddNumberToObject(obj, key, *value);
	}
    void JHelper::addNullableBool(cJSON* obj, const char* key, std::optional<bool> value){
		if(!value.has_value())
		{
			cJSON_AddNullToObject(obj, key);
			return;
		}
		cJSON_AddBoolToObject(obj, key, *value);
	}
void JHelper::printJsonKeysAndValues(cJSON* json) {
    if (!json || !cJSON_IsObject(json)) {
        printf("Not a JSON object.\n");
        return;
    }

    for (cJSON* item = json->child; item != NULL; item = item->next) {
        const char* key = (item->string ? item->string : "<no-key>");

        if (cJSON_IsString(item)) {
            const char* val = (item->valuestring ? item->valuestring : "<null>");
            printf("Key: %s, Value (string): %s\n", key, val);
        }
        else if (cJSON_IsNumber(item)) {
            // Note: on ESP-IDF, enable float printf if needed (CONFIG_NEWLIB_STDOUT_FLOAT)
            printf("Key: %s, Value (number): %f\n", key, item->valuedouble);
        }
        else if (cJSON_IsBool(item)) {
            printf("Key: %s, Value (bool): %s\n", key, cJSON_IsTrue(item) ? "true" : "false");
        }
        else if (cJSON_IsArray(item)) {
            printf("Key: %s, Value (array)\n", key);
        }
        else if (cJSON_IsObject(item)) {
            printf("Key: %s, Value (object)\n", key);
        }
        else if (cJSON_IsNull(item)) {
            printf("Key: %s, Value: null\n", key);
        } else {
            printf("Key: %s, Value: <unknown type>\n", key);
        }
    }
}