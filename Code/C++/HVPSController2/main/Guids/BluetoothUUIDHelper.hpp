#ifndef BLUETOOTH_UUID_HELPER_HPP
#define BLUETOOTH_UUID_HELPER_HPP

#include <iostream>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstdlib>

class BluetoothUUIDHelper {
public:
    // Public method to parse the Bluetooth UUID string and return a byte array
    static std::array<uint8_t, 16> parseBluetoothUUID(const std::string& uuid_str, bool reverse);
    private:
    // Private method to parse two characters into a byte
    static uint8_t parseTwoCharactersToByte(char a, char b);
    // Private method to convert a hex character to a byte
    static uint8_t hexCharacterToByte(char c);
    // Private method to validate the UUID format
    static bool isValidUUIDFormat(const std::string& uuid_str);
    
};
#endif // BLUETOOTH_UUID_HELPER_HPP
