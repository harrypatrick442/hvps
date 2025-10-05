#include "BluetoothUUID.hpp"
#include "../../Logging/Log.hpp"
#include <esp_log.h>

// 🛠 General template implementation (works for all sizes)
template <size_t size>
BluetoothUUID<size>::BluetoothUUID(const std::string& uuid_str, bool bigEndian) : isBigEndian(bigEndian) {
    if (uuid_str.length() != size * 2 + (size > 2 ? (size / 2 - 1) : 0)) {
        throw std::invalid_argument("Invalid UUID string length");
    }

    size_t uuidIndex = 0;
    for (size_t i = 0; i < uuid_str.length(); ++i) {
        if (uuid_str[i] == '-') continue;

        if (uuidIndex >= size) {
            throw std::invalid_argument("UUID string is too long");
        }

        uuid[uuidIndex++] = parseTwoCharactersToByte(uuid_str[i], uuid_str[++i]);
    }

    if (uuidIndex != size) {
        throw std::invalid_argument("UUID string is too short");
    }
}
template <size_t size>
std::array<uint8_t, size> BluetoothUUID<size>::toBigEndian() const {
    if (isBigEndian) {
        return uuid;
    } else {
        std::array<uint8_t, size> reversed_uuid;
        std::reverse_copy(uuid.begin(), uuid.end(), reversed_uuid.begin());
        return reversed_uuid;
    }
}

template <size_t size>
std::array<uint8_t, size> BluetoothUUID<size>::toLittleEndian() const {
    if (!isBigEndian) {
        return uuid;
    } else {
        std::array<uint8_t, size> reversed_uuid;
        std::reverse_copy(uuid.begin(), uuid.end(), reversed_uuid.begin());
        return reversed_uuid;
    }
}

template <size_t size>
void BluetoothUUID<size>::log(const char* tag) const {
    ESP_LOG_BUFFER_HEX(tag, uuid.data(), uuid.size());
}

template <size_t size>
uint8_t BluetoothUUID<size>::hexCharacterToByte(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    Log::Error("BluetoothUUID", "Invalid hex character: %c", c);
    abort();
    return 0;
}

template <size_t size>
uint8_t BluetoothUUID<size>::parseTwoCharactersToByte(char high, char low) {
    return (hexCharacterToByte(high) << 4) | hexCharacterToByte(low);
}

// 🔥 Explicit instantiations to ensure compilation
template class BluetoothUUID<ESP_UUID_LEN_128>;
template class BluetoothUUID<ESP_UUID_LEN_32>;
template class BluetoothUUID<ESP_UUID_LEN_16>;
