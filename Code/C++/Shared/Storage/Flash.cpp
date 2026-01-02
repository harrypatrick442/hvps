#include "./Flash.hpp"
#include "../Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "esp_err.h"
#include "nvs_flash.h" // Ensure ESP-IDF include path is set correctly
#include <cstring>


bool Flash::_isInitialized = false;
bool Flash::getIsInitialized(){
    return _isInitialized;
}

void Flash::initialize() {
    if (_isInitialized) {
        LOG_WARN("NVS is already initialized");
        return;
    }

    esp_err_t ret;
    // Initialize NVS
    ret = nvs_flash_init();
    bool noFreePages = ret == ESP_ERR_NVS_NO_FREE_PAGES;
    bool newVersionFound = ret == ESP_ERR_NVS_NEW_VERSION_FOUND;
    if (noFreePages || newVersionFound) {
        if (noFreePages) {
            LOG_WARN("Erasing NVS because of no free pages");
        }
        if (newVersionFound) {
            LOG_WARN("Erasing NVS because of new version found");
        }
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    _isInitialized = true;
}
bool Flash::setFloat(const char* namespaceName, const char* key, float value) {
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    // Store float as uint64_t to preserve exact binary representation
    uint32_t raw;
    std::memcpy(&raw, &value, sizeof(float));
    err = nvs_set_u32(handle, key, raw);
    if (err != ESP_OK) {
        nvs_close(handle);
        LOG_WARN("Failed to set float: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    if (err != ESP_OK) {
        LOG_WARN("Failed to commit float: %s", esp_err_to_name(err));
        return false;
    }
    return true;
}


// Retrieve a float from NVS
bool Flash::getFloat(const char* namespaceName, const char* key, float &outValue) {
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
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

    uint32_t raw;
    err = nvs_get_u32(handle, key, &raw);
    nvs_close(handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        //LOG_WARN("Float value not found for key '%s'", key);
        return false;
    }
    if (err != ESP_OK) {
        LOG_WARN("Failed to read float: %s", esp_err_to_name(err));
        return false;
    }

    std::memcpy(&outValue, &raw, sizeof(float));
    return true;
}


// Store a string in NVS.
// If the value is empty and allowEmptyErase is true, the key will be erased.
bool Flash::setString(const char* namespaceName, const char* key,
                      const std::string& value,
                      bool allowEmptyErase /* = true */)
{
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    // Handle empty string case — optionally erase key
    if (value.empty() && allowEmptyErase) {
        err = nvs_erase_key(handle, key);
		if (err == ESP_OK) {
			err = nvs_commit(handle);
            nvs_close(handle);
			if (err != ESP_OK) {
				LOG_WARN("Failed to commit NVS changes for key '%s': %s",
						  key, esp_err_to_name(err));
				return false;
			}
			return true;
		}
        if (err == ESP_ERR_NVS_NOT_FOUND) {
			nvs_close(handle);
			return true;
        }
		nvs_close(handle);
		LOG_WARN("Failed to erase key '%s': %s", key, esp_err_to_name(err));
		return false;
    } 
	
    // Store normally
	err = nvs_set_str(handle, key, value.c_str());
	if (err != ESP_OK) {
		nvs_close(handle);
		LOG_WARN("Failed to set string for key '%s': %s",
				  key, esp_err_to_name(err));
		return false;
	}

    // Commit the changes
    err = nvs_commit(handle);
    nvs_close(handle);
    if (err != ESP_OK) {
        LOG_WARN("Failed to commit NVS changes for key '%s': %s",
                  key, esp_err_to_name(err));
        return false;
    }
    return true;
}


// Retrieve a string from NVS into std::string.
// If maxLength is 0, the full string is read regardless of size.
// If maxLength > 0 and the stored string is longer, it is truncated safely.
bool Flash::getString(const char* namespaceName, const char* key,
    std::string& outValue, size_t maxLength /* = 0 */, bool allowTruncate /* = true */)
{
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return false; // namespace never created
    }
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    // Step 1: find out how big the stored string is
    size_t requiredSize = 0;
    err = nvs_get_str(handle, key, nullptr, &requiredSize);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        nvs_close(handle);
        return false; // key missing
    }
    if (err != ESP_OK) {
        nvs_close(handle);
        LOG_WARN("Failed to determine string length for key '%s': %s",
                  key, esp_err_to_name(err));
        return false;
    }

    // Handle empty string (NVS stores '\0')
    if (requiredSize <= 1) {
        outValue.clear();
        nvs_close(handle);
        return true;
    }

    // Step 2: allocate buffer
    size_t bufferSize = requiredSize;
    if (maxLength > 0 && requiredSize > maxLength) {
        if (!allowTruncate) {
            LOG_WARN("String for key '%s' was too long (%zu > %zu bytes) and allowTruncate was false.",
                      key, requiredSize, maxLength);
            nvs_close(handle);
            return false;
        }
        LOG_WARN("Stored string too long for key '%s' (%zu > %zu bytes). Truncating.",
                  key, requiredSize, maxLength);
        bufferSize = maxLength;
    }

    std::vector<char> buffer(bufferSize);
    err = nvs_get_str(handle, key, buffer.data(), &bufferSize);
    nvs_close(handle);

    if (err != ESP_OK) {
        LOG_WARN("Failed to read string for key '%s': %s",
                  key, esp_err_to_name(err));
        return false;
    }

    // Defensive: ensure null termination
    if (!buffer.empty())
        buffer.back() = '\0';

    outValue.assign(buffer.data());
    return true;
}
bool Flash::getCharStringOnHeap(
    const char* namespaceName,
    const char* key,
    char*& outStr,
    CleanupBucket& cleanupBucket,
    size_t maxLength /*= 0*/,
    bool allowTruncate /*= true*/)
{
    outStr = nullptr;

    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return false; // namespace never created
    }
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    // Step 1: Query required size (includes null terminator)
    size_t requiredSize = 0;
    err = nvs_get_str(handle, key, nullptr, &requiredSize);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        nvs_close(handle);
        return false; // key missing
    }
    if (err != ESP_OK) {
        nvs_close(handle);
        LOG_WARN("Failed to determine string length for key '%s': %s",
                  key, esp_err_to_name(err));
        return false;
    }

    if (requiredSize <= 1) {
        // Empty string
        outStr = new char[1];
        outStr[0] = '\0';
        cleanupBucket.addDeleteArray(outStr);
        nvs_close(handle);
        return true;
    }

    size_t bufferSize = requiredSize;
    if (maxLength > 0 && bufferSize > maxLength) {
        if (!allowTruncate) {
            LOG_WARN("Stored string too long for key '%s' (%zu > %zu bytes) and allowTruncate is false",
                      key, bufferSize, maxLength);
            nvs_close(handle);
            return false;
        }
        LOG_WARN("Truncating stored string for key '%s' from %zu to %zu bytes",
                  key, bufferSize, maxLength);
        bufferSize = maxLength;
    }

    outStr = new char[bufferSize]; // includes space for null terminator
    cleanupBucket.addDeleteArray(outStr);

    err = nvs_get_str(handle, key, outStr, &bufferSize);
    nvs_close(handle);

    if (err != ESP_OK) {
        LOG_WARN("Failed to read string for key '%s': %s",
                  key, esp_err_to_name(err));
        return false;
    }

    // Defensive: ensure null termination
    outStr[bufferSize - 1] = '\0';
    return true;
}

bool Flash::erase(const char* namespaceName, const char* key)
{
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }
    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }
	err = nvs_erase_key(handle, key);
	if (err == ESP_ERR_NVS_NOT_FOUND) {
		nvs_close(handle);
		return true;
	}
	if (err != ESP_OK) {
		nvs_close(handle);
		LOG_WARN("Failed to erase key '%s': %s", key, esp_err_to_name(err));
		return false;
	}
	err = nvs_commit(handle);
	nvs_close(handle);
	if (err != ESP_OK) {
		LOG_WARN("Failed to commit NVS changes for key '%s': %s",
				  key, esp_err_to_name(err));
		return false;
	}
	return true;
} 

bool Flash::hasKey(const char* namespaceName, const char* key)
{
    if (!_isInitialized) {
        SAFE_ABORT(NVS_NOT_INITIALIZED);
        return false;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(namespaceName, NVS_READONLY, &handle);

    // Namespace never created => no keys
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return false;
    }

    if (err != ESP_OK) {
        LOG_WARN(FAILED_OPEN_NAMESPACE, esp_err_to_name(err));
        return false;
    }

    // We don't care about the type, only existence.
    // Try to read as string just to probe existence.
    size_t requiredSize = 0;
    err = nvs_get_str(handle, key, nullptr, &requiredSize);
    nvs_close(handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        // Key genuinely doesn't exist
        return false;
    }

    if (err == ESP_OK || err == ESP_ERR_NVS_TYPE_MISMATCH) {
        // OK: key exists (string or some other type)
        return true;
    }

    // Any other error is a real failure
    LOG_WARN("Failed to probe existence of key '%s': %s",
              key, esp_err_to_name(err));
    return false;
}