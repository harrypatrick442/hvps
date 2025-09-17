#include "./Flash.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "esp_err.h"
#include "nvs_flash.h" // Ensure ESP-IDF include path is set correctly
#include <cstring>
const char* Flash::TAG = "Flash";
bool Flash::_isInitialized = false;
bool Flash::getIsInitialized(){
    return _isInitialized;
}

void Flash::initialize() {
    if (_isInitialized) {
        Log::Info(TAG, "NVS is already initialized");
        return;
    }

    esp_err_t ret;
    Log::Info(TAG, "Initializing NVS");
    // Initialize NVS
    ret = nvs_flash_init();
    bool noFreePages = ret == ESP_ERR_NVS_NO_FREE_PAGES;
    bool newVersionFound = ret == ESP_ERR_NVS_NEW_VERSION_FOUND;
    if (noFreePages || newVersionFound) {
        if (noFreePages) {
            Log::Info(TAG, "Erasing NVS because of no free pages");
        }
        if (newVersionFound) {
            Log::Info(TAG, "Erasing NVS because of new version found");
        }
        ret = nvs_flash_erase();
		if(ret!=ESP_OK){
			Aborter::safeAbort(TAG, "Failed to erase flash: %s", esp_err_to_name(ret));
			return;
		}
        ret = nvs_flash_init();
    }
	if(ret!=ESP_OK){
		Aborter::safeAbort(TAG, "Failed to init flash", esp_err_to_name(ret));
		return;
	}
        

    _isInitialized = true;
    Log::Info(TAG, "NVS initialized successfully");
}

// Store a double in NVS
bool Flash::setDouble(const char* namespaceName, const char* key, double value) {
    if (!_isInitialized) {
        Aborter::safeAbort(TAG, "NVS not initialized");
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        Log::Warn(TAG, "Failed to open NVS namespace for setDouble: %s", esp_err_to_name(err));
        return false;
    }

    // Store double as uint64_t to preserve exact binary representation
    uint64_t raw;
    std::memcpy(&raw, &value, sizeof(double));
    err = nvs_set_u64(handle, key, raw);
    if (err != ESP_OK) {
        nvs_close(handle);
        Log::Warn(TAG, "Failed to set double: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    if (err != ESP_OK) {
        Log::Warn(TAG, "Failed to commit double: %s", esp_err_to_name(err));
        return false;
    }

    Log::Info(TAG, "Double value stored successfully");
    return true;
}

// Retrieve a double from NVS
bool Flash::getDouble(const char* namespaceName, const char* key, double &outValue) {
    if (!_isInitialized) {
        Aborter::safeAbort(TAG, "NVS not initialized");
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        //Aborter::safeAbort(TAG, "Failed to open NVS namespace for getDouble: %s", esp_err_to_name(err));
        return false;
    }

    uint64_t raw;
    err = nvs_get_u64(handle, key, &raw);
    nvs_close(handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        //Log::Warn(TAG, "Double value not found for key '%s'", key);
        return false;
    }
    if (err != ESP_OK) {
        Log::Warn(TAG, "Failed to read double: %s", esp_err_to_name(err));
        return false;
    }

    std::memcpy(&outValue, &raw, sizeof(double));
    return true;
}