#ifndef I_FONT_LIBRARY_H
#define I_FONT_LIBRARY_H
#include <vector>
#include <string>
class IFontLibrary {
public:
    virtual ~IFontLibrary() = default;
    virtual uint8_t getCharWidth() = 0;
    virtual uint8_t getCharcterHeight() = 0;
    virtual std::vector<uint8_t> GetBytes(const std::string& str) = 0;
    virtual std::vector<bool> GetBools(const std::string& str) = 0;
};

#endif // I_FONT_LIBRARY_H