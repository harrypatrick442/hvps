#include "MonochromeDisplayBuffer.hpp"
#include <cstdint>
#include <cstdio>
#include <vector>
#include "../Logging/Log.hpp"
const char* MonochromeDisplayBuffer::TAG = "MonochromeDisplayBuffer";
MonochromeDisplayBuffer::MonochromeDisplayBuffer(IMonochromeDisplay* display){
    _display = display;
    uint8_t size = 
        _display->getNPages() 
        * _display->getNColumns();
    _buffer = new uint8_t[size];
}
void MonochromeDisplayBuffer::SetRegion(
    uint8_t x, uint8_t y, uint8_t width, uint8_t height,
     const bool* pixelData){
    int nDisplayColumns = _display->getNColumns();
    int nDisplayPages = _display->getNPages();
    int nDisplayRows = nDisplayPages*8;   
    if(y + height>nDisplayRows){
        height = nDisplayRows - y;
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Attempted to write outside of display buffer with y=%d and height=%d", y, height);
        Log::Error(TAG, buffer);

    }
    if(height<=0){
        return;
    }
    if(x + width > nDisplayColumns){
        width = nDisplayColumns - x;
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Attempted to write outside of display buffer with x=%d and width=%d", x, width);
        Log::Error(TAG, buffer);
    }
    if(width<=0){
        return;
    }
    int columnIndex = x;
    int rowIndex = y;
    uint8_t startPage = y / 8;
    uint8_t endPage = (y + height - 1) / 8;
    uint8_t pageIndex = startPage;
    int pixelDataIndex=0;
    while(rowIndex<height){
        for(int j=0; j<width; j++){
            int bufferIndex = pageIndex * width + columnIndex;
            int rowIndexModEight = y % 8;
            uint8_t mask = 1 << rowIndexModEight;
            uint8_t inverseMask = ~mask;
            bool value = pixelData[pixelDataIndex++];
            _buffer[bufferIndex] = 
                (_buffer[bufferIndex] & inverseMask) 
                | (value ? mask : 0);
            columnIndex++;
            pixelDataIndex++;
        }
        rowIndex++;
        if(rowIndex%8==0){
            pageIndex++;
            columnIndex = x;
        }
    }
    _display->UpdatePixels(
        startPage, endPage,
        x, 
        x+width-1,
        nDisplayColumns,
        _buffer
    );
}