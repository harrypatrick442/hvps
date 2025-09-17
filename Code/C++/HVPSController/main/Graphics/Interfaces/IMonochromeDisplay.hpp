#ifndef I_MONOCHROME_DISPLAY_H
#define I_MONOCHROME_DISPLAY_H

class IMonochromeDisplay {
public:
    virtual ~IMonochromeDisplay() = default;
    virtual uint8_t getNPages() = 0;
    virtual uint8_t getNColumns() = 0;
    virtual void InitializeForMonochromeDisplayBuffer() = 0;
    virtual void UpdatePixels(
        uint8_t startPage, uint8_t endPage,
        uint8_t startColumn, uint8_t endColumn,
        uint8_t totalBufferWidth,
        const uint8_t* pixelData) = 0;
};

#endif // I_MONOCHROME_DISPLAY_H