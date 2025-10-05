#ifndef SSD1306_H
#define SSD1306_H

#include "esp_log.h"
#include "../../Communication/Interfaces/IReadWrite.hpp"
#include "../../Graphics/Interfaces/IMonochromeDisplay.hpp"
#include "../../Graphics/Enums/MemoryAddressingMode.hpp"
class SSD1306 :public IMonochromeDisplay{
public:
    SSD1306(IReadWrite& readWrite);
    uint8_t getNPages() override;
    uint8_t getNColumns() override;
    void SetContrastControl(uint8_t value);
    void EntireDisplayOn(bool onElseRAM);
    void SetNormalInverseDisplay(bool normalElseInverse);
    void SetDisplayOnOff(bool onElseOff);
    void ContinuousHorizontalScrollSetup(uint8_t startPageAddress, uint8_t timeIntervalScrollStep, uint8_t endPageAddress, bool leftElseRight);
    void ContinuousVerticalAndHorizontalScrollSetup(uint8_t startPageAddress, uint8_t timeIntervalScrollStep, uint8_t endPageAddress, uint8_t verticalScrollingOffset, bool leftElseRight);
    void DeactivateScroll();//Call this
    void ActivateScroll();
    void SetVerticalScrollArea(uint8_t topFixedArea, uint8_t scrollAreaHeight);
    void SetLowerColumnStartAddressForPageAddressingMode(uint8_t address);
    void SetHigherColumnStartAddressForPageAddressingMode(uint8_t address);
    void SetMemoryAddressingMode(MemoryAddressingMode memoryAddressingMode);
    void SetMemoryAddressingMode(uint8_t mode);
    void SetColumnAddress(uint8_t startAddress, uint8_t endAddress);
    void SetPageAddress(uint8_t startAddress, uint8_t endAddress);
    void SetPageStartAddressForPageAddressingMode(uint8_t address);
    void SetDisplayStartLine(uint8_t startLine);
    void SetSegmentRemap(bool remap);
    void SetMultiplexRatio(uint8_t ratio);
    void SetCOMOutputScanDirection(bool remap);
    void SetDisplayOffset(uint8_t offset);
    void SetCOMPinsHardwareConfiguration(uint8_t config);
    void SetClockDivideRatioOscillatorFrequency(uint8_t divideRatio, uint8_t oscillatorFrequency);
    void SetPreChargePeriod(uint8_t period);
    void SetVcomhDeselectLevel(uint8_t level);
    void SetChargePumpEnabled(bool enableChargePump);
    void NOP();
    uint8_t StatusRegisterRead();
    void SetPixel(uint8_t x, uint8_t y, bool on);
    void UpdatePixels(
    uint8_t startPage, uint8_t endPage,
     uint8_t startColumn, uint8_t endColumn,
     uint8_t totalBufferWidth,
      const uint8_t* pixelData);
    void SendDataFrom(const uint8_t* data, uint16_t startIndex, uint16_t size);
    void InitializeForMonochromeDisplayBuffer() override;
    static const char* TAG;
private:
    IReadWrite& _readWrite;
    static const uint8_t ADDRESS;
    static const uint8_t COMMAND_CONTRAST_CONTROL;
    static const uint8_t COMMAND_ENTIRE_DISPLAY_ON_ON;
    static const uint8_t COMMAND_ENTIRE_DISPLAY_ON_RAM;
    static const uint8_t COMMAND_SET_NORMAL_INVERSE_DISPLAY_NORMAL;
    static const uint8_t COMMAND_SET_NORMAL_INVERSE_DISPLAY_INVERSE;
    static const uint8_t COMMAND_SET_DISPLAY_ON_OFF_ON;
    static const uint8_t COMMAND_SET_DISPLAY_ON_OFF_OFF;
    static const uint8_t COMMAND_DEACTIVATE_SCROLL;
    static const uint8_t COMMAND_ACTIVATE_SCROLL;
    static const uint8_t COMMAND_SET_VERTICAL_SCROLL_AREA;
    static const uint8_t COMMAND_SET_LOWER_COLUMN_START_ADDRESS;
    static const uint8_t COMMAND_SET_HIGHER_COLUMN_START_ADDRESS;
    static const uint8_t COMMAND_SET_MEMORY_ADDRESSING_MODE;
    static const uint8_t COMMAND_SET_COLUMN_ADDRESS;
    static const uint8_t COMMAND_SET_PAGE_ADDRESS;
    static const uint8_t COMMAND_SET_PAGE_START_ADDRESS;
    static const uint8_t COMMAND_SET_DISPLAY_START_LINE;
    static const uint8_t COMMAND_SET_SEGMENT_REMAP;
    static const uint8_t COMMAND_SET_MULTIPLEX_RATIO;
    static const uint8_t COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION;
    static const uint8_t COMMAND_SET_DISPLAY_OFFSET;
    static const uint8_t COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION;
    static const uint8_t COMMAND_SET_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY;
    static const uint8_t COMMAND_SET_PRECHARGE_PERIOD;
    static const uint8_t COMMAND_SET_VCOMH_DESELECT_LEVEL;
    static const uint8_t COMMAND_NOP;
    static const uint8_t COMMAND_SET_CHARGE_PUMP_ENABLED;
    static const uint8_t COMMAND_SET_CHARGE_PUMP_ENABLED_ENABLED;
    static const uint8_t COMMAND_SET_CHARGE_PUMP_ENABLED_DISABLED;
    static const uint8_t HORIZONTAL_ADDRESSING_MODE,
        VERTICAL_ADDRESSING_MODE,
        PAGE_ADDRESSING_MODE;
};

#endif // SSD1306_H