#ifndef BLUETOOTHUUID_HPP
#define BLUETOOTHUUID_HPP

#include <string>
#include <array>
#include <algorithm>
#include <stdexcept>
#include "esp_bt_defs.h" // For ESP_UUID_LEN_128, ESP_UUID_LEN_32, ESP_UUID_LEN_16

template <size_t size>
class BluetoothUUID {
public:
    BluetoothUUID(const std::string& uuid_str, bool bigEndian = true);
    
    std::array<uint8_t, size> toBigEndian() const;
    std::array<uint8_t, size> toLittleEndian() const;
    void log(const char* tag) const;

private:
    std::array<uint8_t, size> uuid;
    bool isBigEndian;

    static uint8_t hexCharacterToByte(char c);
    static uint8_t parseTwoCharactersToByte(char high, char low);
};

// Explicit template specializations (not needed, generic template works for all)
template <>
BluetoothUUID<ESP_UUID_LEN_128>::BluetoothUUID(const std::string& uuid_str, bool bigEndian);
template <>
BluetoothUUID<ESP_UUID_LEN_32>::BluetoothUUID(const std::string& uuid_str, bool bigEndian);
template <>
BluetoothUUID<ESP_UUID_LEN_16>::BluetoothUUID(const std::string& uuid_str, bool bigEndian);

#endif // BLUETOOTHUUID_HPP
