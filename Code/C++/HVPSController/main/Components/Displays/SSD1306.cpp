#include "SSD1306.hpp"
#include <cstring>
#include "../../Logging/Log.hpp"
const uint8_t SSD1306::ADDRESS = 0x3C;
const uint8_t SSD1306::COMMAND_CONTRAST_CONTROL = 0x81;
const uint8_t SSD1306::COMMAND_ENTIRE_DISPLAY_ON_ON = 0xA5;
const uint8_t SSD1306::COMMAND_ENTIRE_DISPLAY_ON_RAM = 0xA4;
const uint8_t SSD1306::COMMAND_SET_NORMAL_INVERSE_DISPLAY_NORMAL = 0xA6;
const uint8_t SSD1306::COMMAND_SET_NORMAL_INVERSE_DISPLAY_INVERSE = 0xA7;
const uint8_t SSD1306::COMMAND_SET_DISPLAY_ON_OFF_ON = 0xAF;
const uint8_t SSD1306::COMMAND_SET_DISPLAY_ON_OFF_OFF = 0xAE;
const uint8_t SSD1306::COMMAND_DEACTIVATE_SCROLL = 0x2E;
const uint8_t SSD1306::COMMAND_ACTIVATE_SCROLL = 0x2F;
const uint8_t SSD1306::COMMAND_SET_VERTICAL_SCROLL_AREA = 0xA3;
const uint8_t SSD1306::COMMAND_SET_LOWER_COLUMN_START_ADDRESS = 0x00;
const uint8_t SSD1306::COMMAND_SET_HIGHER_COLUMN_START_ADDRESS = 0x10;
const uint8_t SSD1306::COMMAND_SET_MEMORY_ADDRESSING_MODE = 0x20;
const uint8_t SSD1306::COMMAND_SET_COLUMN_ADDRESS = 0x21;
const uint8_t SSD1306::COMMAND_SET_PAGE_ADDRESS = 0x22;
const uint8_t SSD1306::COMMAND_SET_PAGE_START_ADDRESS = 0xB0;
const uint8_t SSD1306::COMMAND_SET_DISPLAY_START_LINE = 0x40;
const uint8_t SSD1306::COMMAND_SET_SEGMENT_REMAP = 0xA0;
const uint8_t SSD1306::COMMAND_SET_MULTIPLEX_RATIO = 0xA8;
const uint8_t SSD1306::COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION = 0xC0;
const uint8_t SSD1306::COMMAND_SET_DISPLAY_OFFSET = 0xD3;
const uint8_t SSD1306::COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDA;
const uint8_t SSD1306::COMMAND_SET_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY = 0xD5;
const uint8_t SSD1306::COMMAND_SET_PRECHARGE_PERIOD = 0xD9;
const uint8_t SSD1306::COMMAND_SET_VCOMH_DESELECT_LEVEL = 0xDB;
const uint8_t SSD1306::COMMAND_NOP = 0xE3;
const uint8_t SSD1306::COMMAND_SET_CHARGE_PUMP_ENABLED = 0x8d;
const uint8_t SSD1306::COMMAND_SET_CHARGE_PUMP_ENABLED_ENABLED=0x14;
const uint8_t SSD1306::COMMAND_SET_CHARGE_PUMP_ENABLED_DISABLED=0x10;
const uint8_t SSD1306::HORIZONTAL_ADDRESSING_MODE=0x0;
const uint8_t SSD1306::VERTICAL_ADDRESSING_MODE=0x01;
const uint8_t SSD1306::PAGE_ADDRESSING_MODE=0b10;
const char* SSD1306::TAG = "SSD130";
SSD1306::SSD1306(IReadWrite& readWrite)
:_readWrite(readWrite){
}
uint8_t SSD1306::getNPages(){
    return 8;
}
uint8_t SSD1306::getNColumns(){
    return 128;
}
void SSD1306::SetContrastControl(uint8_t value) {
    uint8_t data[2];
    data[0] = COMMAND_CONTRAST_CONTROL; // First byte is the fixed command
    data[1] = value;                    // Second byte is the provided value
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::EntireDisplayOn(bool onElseRAM) {
    uint8_t command = onElseRAM ? COMMAND_ENTIRE_DISPLAY_ON_ON : COMMAND_ENTIRE_DISPLAY_ON_RAM;
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetNormalInverseDisplay(bool normalElseInverse) {
    uint8_t command = normalElseInverse ? 
    COMMAND_SET_NORMAL_INVERSE_DISPLAY_NORMAL
    :COMMAND_SET_NORMAL_INVERSE_DISPLAY_INVERSE;
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetDisplayOnOff(bool onElseOff) {
    uint8_t command = onElseOff ? COMMAND_SET_DISPLAY_ON_OFF_ON : COMMAND_SET_DISPLAY_ON_OFF_OFF;
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::ContinuousHorizontalScrollSetup(
    uint8_t startPageAddress, uint8_t timeIntervalScrollStep,
    uint8_t endPageAddress, bool leftElseRight) {
    uint8_t data[7];
    data[0] = leftElseRight ? 0x26 : 0x27; // Set the scroll direction
    data[1] = 0x00;                        // Dummy byte
    data[2] = startPageAddress;            // Start page address
    data[3] = timeIntervalScrollStep;      // Time interval between each scroll step
    data[4] = endPageAddress;              // End page address
    data[5] = 0x00;                        // Dummy byte
    data[6] = 0xFF;                        // Dummy byte

    _readWrite.write(
        ADDRESS, 
        data, 
        7
    );
}

void SSD1306::ContinuousVerticalAndHorizontalScrollSetup(
    uint8_t startPageAddress, uint8_t timeIntervalScrollStep,
    uint8_t endPageAddress, uint8_t verticalScrollingOffset,
     bool leftElseRight) {
    uint8_t data[8];
    data[0] = leftElseRight ? 0x29 : 0x2A; // Set the scroll direction
    data[1] = 0x00;                        // Dummy byte
    data[2] = startPageAddress;            // Start page address
    data[3] = timeIntervalScrollStep;      // Time interval between each scroll step
    data[4] = endPageAddress;              // End page address
    data[5] = verticalScrollingOffset;     // Vertical scrolling offset
    data[6] = 0x00;                        // Dummy byte
    data[7] = 0xFF;                        // Dummy byte

    _readWrite.write(
        ADDRESS, 
        data, 
        8
    );
}

void SSD1306::DeactivateScroll() {
    uint8_t command = COMMAND_DEACTIVATE_SCROLL;
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::ActivateScroll() {
    uint8_t command = COMMAND_ACTIVATE_SCROLL;
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetVerticalScrollArea(uint8_t topFixedArea, uint8_t scrollAreaHeight) {
    uint8_t data[3];
    data[0] = COMMAND_SET_VERTICAL_SCROLL_AREA;
    data[1] = topFixedArea;
    data[2] = scrollAreaHeight;
    _readWrite.write(ADDRESS, data, 3);
}

void SSD1306::SetLowerColumnStartAddressForPageAddressingMode(uint8_t address) {
    uint8_t command = COMMAND_SET_LOWER_COLUMN_START_ADDRESS | (address & 0x0F);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetHigherColumnStartAddressForPageAddressingMode(uint8_t address) {
    uint8_t command = COMMAND_SET_HIGHER_COLUMN_START_ADDRESS | ((address >> 4) & 0x0F);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetMemoryAddressingMode(MemoryAddressingMode memoryAddressingMode) {
    uint8_t mode = HORIZONTAL_ADDRESSING_MODE;
    switch(memoryAddressingMode){
        case MemoryAddressingMode::Horizontal:
            mode = HORIZONTAL_ADDRESSING_MODE;
            break;
        case MemoryAddressingMode::Vertical:
            mode = VERTICAL_ADDRESSING_MODE;
            break;
        case MemoryAddressingMode::Page:
            mode = PAGE_ADDRESSING_MODE;
            break;
    }
    SetMemoryAddressingMode(mode);
}
void SSD1306::SetMemoryAddressingMode(uint8_t mode) {
    uint8_t data[2];
    data[0] = COMMAND_SET_MEMORY_ADDRESSING_MODE;
    data[1] = mode;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetColumnAddress(uint8_t startAddress, uint8_t endAddress) {
    uint8_t data[3];
    data[0] = COMMAND_SET_COLUMN_ADDRESS;
    data[1] = startAddress;
    data[2] = endAddress;
    _readWrite.write(ADDRESS, data, 3);
}

void SSD1306::SetPageAddress(uint8_t startAddress, uint8_t endAddress) {
    uint8_t data[3];
    data[0] = COMMAND_SET_PAGE_ADDRESS;
    data[1] = startAddress;
    data[2] = endAddress;
    _readWrite.write(ADDRESS, data, 3);
}

void SSD1306::SetPageStartAddressForPageAddressingMode(uint8_t address) {
    uint8_t command = COMMAND_SET_PAGE_START_ADDRESS | (address & 0x07);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetDisplayStartLine(uint8_t startLine) {
    uint8_t command = COMMAND_SET_DISPLAY_START_LINE | (startLine & 0x3F);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetSegmentRemap(bool remap) {
    uint8_t command = COMMAND_SET_SEGMENT_REMAP | (remap ? 0x01 : 0x00);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetMultiplexRatio(uint8_t ratio) {
    uint8_t data[2];
    data[0] = COMMAND_SET_MULTIPLEX_RATIO;
    data[1] = ratio;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetCOMOutputScanDirection(bool remap) {
    uint8_t command = COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION | (remap ? 0x08 : 0x00);
    _readWrite.write(ADDRESS, &command, 1);
}

void SSD1306::SetDisplayOffset(uint8_t offset) {
    uint8_t data[2];
    data[0] = COMMAND_SET_DISPLAY_OFFSET;
    data[1] = offset;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetCOMPinsHardwareConfiguration(uint8_t config) {
    uint8_t data[2];
    data[0] = COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION;
    data[1] = config;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetClockDivideRatioOscillatorFrequency(uint8_t divideRatio, uint8_t oscillatorFrequency) {
    uint8_t data[2];
    data[0] = COMMAND_SET_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY;
    data[1] = (divideRatio & 0x0F) | ((oscillatorFrequency & 0x0F) << 4);
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetPreChargePeriod(uint8_t period) {
    uint8_t data[2];
    data[0] = COMMAND_SET_PRECHARGE_PERIOD;
    data[1] = period;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetVcomhDeselectLevel(uint8_t level) {
    uint8_t data[2];
    data[0] = COMMAND_SET_VCOMH_DESELECT_LEVEL;
    data[1] = level;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::SetChargePumpEnabled(bool enableChargePump) {
    uint8_t data[2];
    data[0] = COMMAND_SET_CHARGE_PUMP_ENABLED;
    data[1] = enableChargePump
        ?COMMAND_SET_CHARGE_PUMP_ENABLED_ENABLED
        :COMMAND_SET_CHARGE_PUMP_ENABLED_DISABLED;
    _readWrite.write(ADDRESS, data, 2);
}

void SSD1306::NOP() {
    uint8_t command = COMMAND_NOP;
    _readWrite.write(ADDRESS, &command, 1);
}

uint8_t SSD1306::StatusRegisterRead() {
    uint8_t status;
    _readWrite.read(ADDRESS, &status, 1);
    return status;
}
void SSD1306::SetPixel(uint8_t x, uint8_t y, bool on) {
    /*Example of how we might go about setting individual pixels.
    Once we set up the direction to colum or row it will auto increment
    pixel locations and we could flush a range of values to a range of pixels using a similar approach
    */
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    uint8_t data[3];
    data[0] = COMMAND_SET_COLUMN_ADDRESS;
    data[1] = x;
    data[2] = x;
    _readWrite.write(ADDRESS, data, 3);
    data[0] = COMMAND_SET_PAGE_ADDRESS;
    data[1] = page;
    data[2] = page;
    _readWrite.write(ADDRESS, data, 3);
    uint8_t pixelData = on ? (1 << bit) : 0x00;
    _readWrite.write(ADDRESS, &pixelData, 1);
}
void SSD1306::UpdatePixels(
    uint8_t startPage, uint8_t endPage,
     uint8_t startColumn, uint8_t endColumn,
     uint8_t totalBufferWidth,
      const uint8_t* pixelData) {
    uint8_t dataSize = endColumn - startColumn + 1;
    uint8_t data[dataSize];
    for(int page = startPage; page<=endPage; page++){
        SetColumnAddress(startColumn, endColumn);
        //Need to call on every cycle because it auto increments

        SetPageAddress(page, page);
        SendDataFrom(data, (totalBufferWidth*page)+startColumn, dataSize);//TODO a write from index so i can directly pass in the entire buffer
    }
}
void SSD1306::SendDataFrom(const uint8_t* data, uint16_t startIndex, uint16_t size) {
    const uint16_t chunkSize = 16; // Define a reasonable chunk size
    uint8_t buffer[chunkSize + 1];
    buffer[0] = 0x40; // 0x40 indicates data
    
    while (size > 0) {
        uint16_t currentChunkSize = (size > chunkSize) ? chunkSize : size;
        memcpy(buffer + 1, &data[startIndex], currentChunkSize);
        _readWrite.write(ADDRESS, buffer, currentChunkSize + 1);
        startIndex += currentChunkSize;
        size -= currentChunkSize;
    }
}
void SSD1306::InitializeForMonochromeDisplayBuffer(){
    /*//Mr copilot please implement here
    Log::Info(TAG, "Initializing SSD1306 for monochrome display buffer");
    SetDisplayOnOff(false);// Display OFF (sleep mode)
    SetClockDivideRatioOscillatorFrequency(0x0,0x8); // Set display clock divide ratio/oscillator frequency
    SetMultiplexRatio(0x3F); // Set multiplex ratio(1 to 64). In this case all rows active
    SetDisplayStartLine(0x00); // Set display offset to no offset
    SetChargePumpEnabled(true); // Enable charge pump regulator
    SetMemoryAddressingMode(HORIZONTAL_ADDRESSING_MODE); // Set Memory Addressing Mode
    SetSegmentRemap(false);// Set segment re-map 0 to 127
    SetCOMOutputScanDirection(false); // Set COM Output Scan Direction
    //sendCommand(0xDA); // Set COM Pins hardware configuration
    SetCOMPinsHardwareConfiguration(0x12);
    SetContrastControl(0x7F); // Set contrast control register
    SetPreChargePeriod(0xF1); // Set pre-charge period
    SetVcomhDeselectLevel(0x40); // Set VCOMH deselect level
    EntireDisplayOn(false); // Display ON in normal mode
    SetNormalInverseDisplay(true);
    DeactivateScroll();
    SetDisplayOnOff(true); // Display ON in normal mode
    Log::Info(TAG, "SSD1306 initialization complete");
    */
    Log::Info(TAG, "Initializing SSD1306 for monochrome display buffer");

    SetDisplayOnOff(false); // Display OFF (sleep mode)
    Log::Info(TAG, "Display OFF");

    SetClockDivideRatioOscillatorFrequency(0x0, 0x8); // Set display clock divide ratio/oscillator frequency
    Log::Info(TAG, "Clock divide ratio/oscillator frequency set");

    SetMultiplexRatio(0x3F); // Set multiplex ratio (1 to 64). In this case, all rows active
    Log::Info(TAG, "Multiplex ratio set");

    SetDisplayStartLine(0x00); // Set display start line to 0
    Log::Info(TAG, "Display start line set");

    SetDisplayOffset(0x00); // Set display offset to no offset
    Log::Info(TAG, "Display offset set");

    SetChargePumpEnabled(true); // Enable charge pump regulator
    Log::Info(TAG, "Charge pump enabled");

    SetMemoryAddressingMode(HORIZONTAL_ADDRESSING_MODE); // Set Memory Addressing Mode
    Log::Info(TAG, "Memory addressing mode set");

    SetSegmentRemap(true); // Set segment re-map (0xA1/0xA0)
    Log::Info(TAG, "Segment re-map set");

    SetCOMOutputScanDirection(true); // Set COM Output Scan Direction (0xC8/0xC0)
    Log::Info(TAG, "COM output scan direction set");

    SetCOMPinsHardwareConfiguration(0x12); // Set COM Pins hardware configuration
    Log::Info(TAG, "COM pins hardware configuration set");

    SetContrastControl(0x7F); // Set contrast control register
    Log::Info(TAG, "Contrast control set");

    SetPreChargePeriod(0xF1); // Set pre-charge period
    Log::Info(TAG, "Pre-charge period set");

    SetVcomhDeselectLevel(0x40); // Set VCOMH deselect level
    Log::Info(TAG, "VCOMH deselect level set");

    EntireDisplayOn(false); // Display ON in normal mode
    Log::Info(TAG, "Entire display ON in normal mode");

    SetNormalInverseDisplay(true); // Set display to normal mode
    Log::Info(TAG, "Display set to normal mode");

    DeactivateScroll(); // Deactivate scrolling
    Log::Info(TAG, "Scrolling deactivated");

    SetDisplayOnOff(true); // Display ON in normal mode
    Log::Info(TAG, "Display ON");

    Log::Info(TAG, "SSD1306 initialization complete");

    // Clear the display buffer
   // ClearDisplay();
    Log::Info(TAG, "Display cleared");

    // Draw some initial pixels to form a pattern
    SetPixel(10, 10, true);
    SetPixel(11, 11, true);
    SetPixel(12, 12, true);
    SetPixel(13, 13, true);
    SetPixel(14, 14, true);
    Log::Info(TAG, "Initial pixels set");

    // Draw a horizontal line
    for (int x = 20; x < 50; x++) {
        SetPixel(x, 20, true);
    }
    Log::Info(TAG, "Horizontal line drawn");

    // Draw a vertical line
    for (int y = 30; y < 50; y++) {
        SetPixel(30, y, true);
    }
    Log::Info(TAG, "Vertical line drawn");

    Log::Info(TAG, "Display content updated");
}