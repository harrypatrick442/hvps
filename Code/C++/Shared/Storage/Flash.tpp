
#include "../Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "esp_err.h"
#include "nvs_flash.h" // Ensure ESP-IDF include path is set correctly
#include <cstring>

template<typename T>
bool Flash::setNumber(const char* namespaceName, const char* key, T value) {
	if (!_isInitialized) {
		SAFE_ABORT("NVS not initialized");
		return false;
	}


	nvs_handle_t handle;
	esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
	if (err != ESP_OK) {
		LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
		return false;
	}


	esp_err_t writeErr = ESP_OK;
	if constexpr (std::is_same_v<T, uint8_t>) {
		writeErr = nvs_set_u8(handle, key, value);
	} else if constexpr (std::is_same_v<T, int8_t>) {
		writeErr = nvs_set_i8(handle, key, value);
	} else if constexpr (std::is_same_v<T, uint16_t>) {
		writeErr = nvs_set_u16(handle, key, value);
	} else if constexpr (std::is_same_v<T, int16_t>) {
		writeErr = nvs_set_i16(handle, key, value);
	} else if constexpr (std::is_same_v<T, uint32_t>) {
		writeErr = nvs_set_u32(handle, key, value);
	} else if constexpr (std::is_same_v<T, int32_t>) {
		writeErr = nvs_set_i32(handle, key, value);
	} else if constexpr (std::is_same_v<T, uint64_t>) {
		writeErr = nvs_set_u64(handle, key, value);
	} else if constexpr (std::is_same_v<T, int64_t>) {
		writeErr = nvs_set_i64(handle, key, value);
	} else if constexpr (std::is_same_v<T, float>) {
		writeErr = nvs_set_u32(handle, key, *reinterpret_cast<uint32_t*>(&value));
	} else if constexpr (std::is_same_v<T, double>) {
		uint64_t raw;
		std::memcpy(&raw, &value, sizeof(double));
		writeErr = nvs_set_u64(handle, key, raw);
	} else {
		static_assert(!sizeof(T*), "Unsupported type in setNumber()");
	}


	if (writeErr != ESP_OK) {
		nvs_close(handle);
		LOG_WARN("Failed to set number: %s", esp_err_to_name(writeErr));
		return false;
	}


	err = nvs_commit(handle);
	nvs_close(handle);
	if (err != ESP_OK) {
		LOG_WARN("Failed to commit NVS number: %s", esp_err_to_name(err));
		return false;
	}


	return true;
}
template<typename T>
bool Flash::getNumber(const char* namespaceName, const char* key, T& outValue) {
	if (!_isInitialized) {
		SAFE_ABORT("NVS not initialized");
		return false;
	}


	nvs_handle_t handle;
	esp_err_t err = nvs_open(namespaceName, NVS_READONLY, &handle);
	if (err != ESP_OK) {
		LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
		return false;
	}

	esp_err_t readErr = ESP_OK;
	if constexpr (std::is_same_v<T, uint8_t>) {
		readErr = nvs_get_u8(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, int8_t>) {
		readErr = nvs_get_i8(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, uint16_t>) {
		readErr = nvs_get_u16(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, int16_t>) {
		readErr = nvs_get_i16(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, uint32_t>) {
		readErr = nvs_get_u32(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, int32_t>) {
		readErr = nvs_get_i32(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, uint64_t>) {
		readErr = nvs_get_u64(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, int64_t>) {
		readErr = nvs_get_i64(handle, key, &outValue);
	} else if constexpr (std::is_same_v<T, float>) {
		uint32_t raw;
		readErr = nvs_get_u32(handle, key, &raw);
		std::memcpy(&outValue, &raw, sizeof(float));
	} else if constexpr (std::is_same_v<T, double>) {
		uint64_t raw;
		readErr = nvs_get_u64(handle, key, &raw);
		std::memcpy(&outValue, &raw, sizeof(double));
	} else {
		static_assert(!sizeof(T*), "Unsupported type in getNumber()");
	}


	nvs_close(handle);


	if (readErr == ESP_ERR_NVS_NOT_FOUND) 
		return false;
	if (readErr != ESP_OK) {
		LOG_WARN("Failed to get number: %s", esp_err_to_name(readErr));
		return false;
	}


	return true;
}

template <typename T>
bool Flash::getArray(const char* namespaceName, const char* key,
                     T*& outArray, size_t& outCount, 
					 CleanupBucket& cleanupBucket)
{
    outArray = nullptr;
    outCount = 0;

    if (!_isInitialized) {
        SAFE_ABORT("NVS not initialized");
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READONLY, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return false;
    }
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    size_t sizeBytes = 0;
    err = nvs_get_blob(handle, key, nullptr, &sizeBytes);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        nvs_close(handle);
        return false;
    }
    if (err != ESP_OK) {
        nvs_close(handle);
        LOG_WARN("Failed to get blob size for key '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    if (sizeBytes % sizeof(T) != 0) {
        nvs_close(handle);
        LOG_WARN("Invalid blob size for key '%s': not divisible by sizeof(T)", key);
        return false;
    }

    size_t count = sizeBytes / sizeof(T);
    T* data = new(std::nothrow) T[count];
    if (!data) {
        nvs_close(handle);
        LOG_WARN("Memory allocation failed for key '%s'", key);
        return false;
    }

    err = nvs_get_blob(handle, key, data, &sizeBytes);
    nvs_close(handle);
    if (err != ESP_OK) {
        delete[] data;
        LOG_WARN("Failed to read blob for key '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    cleanupBucket.addDeleteArray(data);
    outArray = data;
    outCount = count;
    return true;
}

template <typename T>
bool Flash::setArray(const char* namespaceName, const char* key, const T* array, size_t count)
{
    if (!_isInitialized) {
        SAFE_ABORT("NVS not initialized");
        return false;
    }

    if (!array || count == 0) {
        LOG_WARN("Invalid array or count for key '%s'", key);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(handle, key, array, sizeof(T) * count);
    if (err != ESP_OK) {
        nvs_close(handle);
        LOG_WARN("Failed to store blob '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    if (err != ESP_OK) {
        LOG_WARN("Failed to commit blob '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    return true;
}
