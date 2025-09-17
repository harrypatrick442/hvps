#ifndef TEXT_REGION_H
#define TEXT_REGION_H

#include <cstdint>
class TextRegion {
public:
    TextRegion(uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
    : _x(x), _y(y), _width(width), _height(height) {}
    // Getter methods
    uint8_t getX() const { return _x; }
    uint8_t getY() const { return _y; }
    uint8_t getWidth() const { return _width; }
    uint8_t getHeight() const { return _height; }

private:
    uint8_t _x;
    uint8_t _y;
    uint8_t _width;
    uint8_t _height;
};
#endif // TEXT_REGION_H