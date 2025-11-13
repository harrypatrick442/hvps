
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "esp_err.h"
#include "nvs_flash.h" // Ensure ESP-IDF include path is set correctly
#include <cstring>

template <typename T>
bool Flash::getArray(const char* namespaceName, const char* key,
                     T*& outArray, size_t& outCount, 
					 CleanupBucket& cleanupBucket)
{
    outArray = nullptr;
    outCount = 0;

    if (!_isInitialized) {
        Aborter::safeAbort(TAG, NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READONLY, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return false;
    }
    if (err != ESP_OK) {
        Log::Warn(TAG, FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
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
        Log::Warn(TAG, "Failed to get blob size for key '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    if (sizeBytes % sizeof(T) != 0) {
        nvs_close(handle);
        Log::Warn(TAG, "Invalid blob size for key '%s': not divisible by sizeof(T)", key);
        return false;
    }

    size_t count = sizeBytes / sizeof(T);
    T* data = new(std::nothrow) T[count];
    if (!data) {
        nvs_close(handle);
        Log::Warn(TAG, "Memory allocation failed for key '%s'", key);
        return false;
    }

    err = nvs_get_blob(handle, key, data, &sizeBytes);
    nvs_close(handle);
    if (err != ESP_OK) {
        delete[] data;
        Log::Warn(TAG, "Failed to read blob for key '%s': %s", key, esp_err_to_name(err));
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
        Aborter::safeAbort(TAG, NVS_NOT_INITIALIZED);
        return false;
    }

    if (!array || count == 0) {
        Log::Warn(TAG, "Invalid array or count for key '%s'", key);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        Log::Warn(TAG, FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(handle, key, array, sizeof(T) * count);
    if (err != ESP_OK) {
        nvs_close(handle);
        Log::Warn(TAG, "Failed to store blob '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    if (err != ESP_OK) {
        Log::Warn(TAG, "Failed to commit blob '%s': %s", key, esp_err_to_name(err));
        return false;
    }

    return true;
}
