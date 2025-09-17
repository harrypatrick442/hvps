#ifndef FONTOLED5X7_HPP
#define FONTOLED5X7_HPP

#include <cstdint>
#include <vector>
#include <string>

class FontOLED5X7 {
public:
    FontOLED5X7();
    void drawText(int x, int y, const std::string& text, int screenWidth, std::vector<uint8_t>& screenBuffer, bool breakWords);
    
private:
    static const uint8_t DATA[];
    static const int CHARACTER_WIDTH = 5;
    static const int CHARACTER_HEIGHT = 7;
    static const int DATA_SIZE;
    static const int INDEX_STAR;

    int getLookupIndex(char c) const;
    void getCharacterBytes(int characterIndex, std::vector<uint8_t>& buffer);
    void drawCharacter(int x, int y, char c, int screenWidth, std::vector<uint8_t>& screenBuffer);
};

#endif // FONTOLED5X7_HPP