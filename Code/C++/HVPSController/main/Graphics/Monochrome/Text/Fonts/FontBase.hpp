#ifndef FONT_BASE_H
#define FONT_BASE_H
#include <cstdint>
#include <stdint.h>
#include <string>
#include <vector>

class FontBase {
    public:
        std::vector<uint8_t> getBytes(const std::string& str);
        std::vector<bool> getBools(const std::string& str);
        uint8_t getCharacterWidth();
        uint8_t getCharacterHeight();

    protected:
        FontBase(uint8_t* data, uint16_t dataLength, uint8_t characterWidth, uint8_t characterHeight);
        virtual uint16_t getLookupIndex(char c) const = 0;
        //Treats data as having one index per character entry. So as a 2D array, it is data[characterIndex][characterWidth]

        const uint8_t* _data;
        const uint16_t _dataLength;
        const uint8_t _characterWidth;
        const uint8_t _characterHeight;
        uint16_t _indexStar;

    private:
        void FontBase::readCharacterBytes(
            uint16_t characterIndex/*Treats data as one index per character*/,
            std::vector<uint8_t>& buffer);
        void FontBase::readCharacterBools(
            uint16_t characterIndex/*Treats data as one index per character*/,
            std::vector<bool>& buffer);
};

#endif // FONT_BASE_H