#ifndef MONOCHROME_DISPLAY_BUFFER_H
#define MONOCHROME_DISPLAY_BUFFER_H
#include "../Graphics/Interfaces/IMonochromeDisplay.hpp"

class MonochromeDisplayBuffer {
public:
    MonochromeDisplayBuffer(
        IMonochromeDisplay* display);
    void SetRegion(
        uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        const bool* pixelData);
    static const char* TAG;

private:
    IMonochromeDisplay* _display;
    uint8_t* _buffer;
    bool* _taken;
};

#endif // MONOCHROME_DISPLAY_BUFFER_H