#include "TextRegion.h"
TextRegion::TextRegion(uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
: x(x), y(y), width(width), height(height) {}
uint8_t TextRegion::getX() const { return x; }
uint8_t TextRegion::getY() const { return y; }
uint8_t TextRegion::getWidth() const { return width; }
uint8_t TextRegion::getHeight() const { return height; }