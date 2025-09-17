#include "./Flash.hpp"
#include "../Logging/Log.hpp"
#include "esp_err.h"
#include "nvs_flash.h" // Ensure ESP-IDF include path is set correctly

bool Flash::_isInitialized = false;
bool Flash::getIsInitialized(){
    return _isInitialized;
}

void Flash::initialize() {
    if (_isInitialized) {
        Log::Info("Flash", "NVS is already initialized");
        return;
    }

    esp_err_t ret;
    Log::Info("Flash", "Initializing NVS");
    // Initialize NVS
    ret = nvs_flash_init();
    bool noFreePages = ret == ESP_ERR_NVS_NO_FREE_PAGES;
    bool newVersionFound = ret == ESP_ERR_NVS_NEW_VERSION_FOUND;
    if (noFreePages || newVersionFound) {
        if (noFreePages) {
            Log::Info("Flash", "Erasing NVS because of no free pages");
        }
        if (newVersionFound) {
            Log::Info("Flash", "Erasing NVS because of new version found");
        }
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    _isInitialized = true;
    Log::Info("Flash", "NVS initialized successfully");
}