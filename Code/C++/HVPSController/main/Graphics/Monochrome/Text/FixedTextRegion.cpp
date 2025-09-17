#include "./FixedTextRegion.hpp"
#include "./Fonts/IFontLibrary.hpp"
#include "../MonochromeDisplayBuffer.hpp"

FixedTextRegion::FixedTextRegion(
    MonochromeDisplayBuffer* displayBuffer,
    uint8_t x, uint8_t y, uint8_t width, uint8_t height,
    IFontLibrary* fontLibrary, const char* text = nullptr)
    : TextRegion(x, y, width, height),
    _displayBuffer(displayBuffer),
     _fontLibrary(fontLibrary) // Assuming TextRegion has a constructor with these parameters
{
    if(text!=nullptr) {
        SetText(text);
    }
}

void FixedTextRegion::SetText(const char* text) {
    std::vector<uint8_t> bytes = _fontLibrary->GetBytes(text);
    _displayBuffer->SetRegion(getX(), getY(), 
        getWidth(), getHeight(), bytes);
    // SetText implementation
}