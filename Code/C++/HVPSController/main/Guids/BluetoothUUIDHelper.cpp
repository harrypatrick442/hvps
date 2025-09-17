#include "./BluetoothUUIDHelper.hpp"
#include <iostream>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstdlib>
#include "../Logging/Log.hpp"
const char* TAG = "BluetoothUUIDHelper";
// Method to parse a Bluetooth UUID string into an array of bytes
std::array<uint8_t, 16> BluetoothUUIDHelper::parseBluetoothUUID(
    const std::string& uuid_str,
    bool reverse) {
    // Validate the UUID format and characters
    if (!isValidUUIDFormat(uuid_str)) {
        abort();
        return {};  // This will never be reached as abort() is called above
    }

    // Prepare the array to hold the parsed UUID bytes
    std::array<uint8_t, 16> uuid_array = {};

    // Parse the first segment (8 characters)
    int charIndex = 0;
    int i = 0;
    while (i < 4) {
        uuid_array[i++] = parseTwoCharactersToByte(uuid_str[charIndex], uuid_str[charIndex+1]);
        charIndex+=2;
    }
    charIndex++; // Skip the dash
    // Parse the second segment (4 characters)
    while (i < 6) {
        uuid_array[i++] = parseTwoCharactersToByte(uuid_str[charIndex], uuid_str[charIndex+1]);
        charIndex+=2;
    }
    charIndex++; // Skip the dash
    // Parse the third segment (4 characters)
    while (i < 8) {
        uuid_array[i++] = parseTwoCharactersToByte(uuid_str[charIndex], uuid_str[charIndex+1]);
        charIndex+=2;
    }
    charIndex++; // Skip the dash
    // Parse the fourth segment (4 characters)
    while (i < 10) {
        uuid_array[i++] = parseTwoCharactersToByte(uuid_str[charIndex], uuid_str[charIndex+1]);
        charIndex+=2;
    }
    charIndex++; // Skip the dash
    // Parse the fifth segment (12 characters)
    while (i < 16) {
        uuid_array[i++] = parseTwoCharactersToByte(uuid_str[charIndex], uuid_str[charIndex+1]);
        charIndex+=2;
    }

    if (reverse) {
        std::reverse(uuid_array.begin(), uuid_array.end());
    }
    return uuid_array;
}
uint8_t BluetoothUUIDHelper::parseTwoCharactersToByte(char a, char b){
    return hexCharacterToByte(a) << 4 | hexCharacterToByte(b);
}
uint8_t BluetoothUUIDHelper::hexCharacterToByte(char c){
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    Log::Error(TAG, "Invalid hex character: %c", c);
    abort();
}
// Check if the UUID format is valid (length and dash positions)
bool BluetoothUUIDHelper::isValidUUIDFormat(const std::string& uuid) {
    size_t size = uuid.size();
    if (size != 36) {
        Log::Error(TAG, "UUID length is incorrect: %d", size);
        return false;
    }

    // Validate dash positions
    for(size_t i=0; i<size; i++) {
        char c = uuid[i];
        if(i==8 || i==13 || i==18 || i==23) {
            if(c != '-') {
                Log::Error(TAG, "UUID dash position is incorrect: %d", i);
                return false;
            }
            continue;
        }
        if (c == '-') {
            Log::Error(TAG, "UUID contains invalid dash at position: %d", i);
            return false;
        }
        if (!std::isxdigit(c)) {
            Log::Error(TAG, "UUID contains invalid character: %c", c);
            return false;
        }
    }
    return true;
}
