#ifndef FIXED_TEXT_REGION_H
#define FIXED_TEXT_REGION_H
#include <cstdint>
#include "../TextRegion.hpp"
#include "./Fonts/IFontLibrary.hpp"

class FixedTextRegion : public TextRegion {
    public:
        FixedTextRegion(
            MonochromeDisplayBuffer* displayBuffer,
            uint8_t x, uint8_t y, uint8_t width,
            uint8_t height, IFontLibrary* fontLibrary, 
            const char* text = nullptr);
        void SetText(const char* text);
    private:
        MonochromeDisplayBuffer* _displayBuffer;
        IFontLibrary* _fontLibrary;
};

#endif // FIXED_TEXT_REGION_H