//For now a single class, will decouple later for different font sizes
#include <cstdint>
#include <vector>
#include <unordered_map>
#include "FontBase.hpp"
FontBase::FontBase(uint8_t* data, uint16_t dataLength, uint8_t characterWidth, uint8_t characterHeight) 
: 
    _data(data), _dataLength(dataLength),
    _characterWidth(characterWidth), _characterHeight(characterHeight) { 
    _indexStar = getLookupIndex('*');
}
void FontBase::readCharacterBytes(
    uint16_t characterIndex/*Treats data as one index per character*/,
    std::vector<uint8_t>& buffer) {
    uint16_t index = characterIndex * _characterWidth;
    uint16_t endIndexExclusive = index + _characterWidth;
    while(index < endIndexExclusive) {
        buffer.push_back(_data[index]);
        index++;
    }
}
void FontBase::readCharacterBools(
    uint16_t characterIndex/*Treats data as one index per character*/,
    std::vector<bool>& buffer) {
    uint8_t buffer [_characterWidth];
    uint8_t i = 0;
    uint16_t index = characterIndex * _characterWidth;
    while(i < _characterWidth) {
        buffer[i] = _data[index];
        index++;
    }
    for (int i = 0; i < _characterWidth; ++i) {
        for (int j = 0; j < _characterHeight; ++j) {
            buffer.push_back(buffer[i] & (1 << j));
        }
    }
    /*The output to this can be th0ought of as an array of bools
    directly corresponding to the visual pixels on the monochrome display*/
}
std::vector<uint8_t> FontBase::getBytes(const std::string& str) {
    std::vector<uint8_t> bytes;
    uint16_t maxLookupIndex = _dataLength-_characterWidth;
    for (char c : str) {
        int lookupIndex = getLookupIndex(c);
        if (lookupIndex >= 0 && lookupIndex <= maxLookupIndex) { // Ensure character is within valid range
            lookupIndex = _indexStar; // Use '*' character if character is out of range
        } 
        readCharacterBytes(lookupIndex, bytes);
    }
    return bytes;
}
std::vector<bool> FontBase::getBools(const std::string& str) {
    std::vector<bool> bools;
    uint16_t maxLookupIndex = _dataLength-_characterWidth;
    for (char c : str) {
        int lookupIndex = getLookupIndex(c);
        if (lookupIndex >= 0 && lookupIndex <= maxLookupIndex) { // Ensure character is within valid range
            lookupIndex = _indexStar; // Use '*' character if character is out of range
        } 
        uint8_t* bytes = readCharacterBools(lookupIndex);
        for (int i = 0; i < _characterWidth; ++i) {
            bools.push_back(bytes & (1 << i));
        }
    }
    return bools;
}