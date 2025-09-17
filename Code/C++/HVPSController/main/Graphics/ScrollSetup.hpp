]#include '/Enums/ScrollType.h'
class ScrollSetup {
public:
    uint8_t startPageAddress;
    uint8_t timeIntervalScrollStep;
    uint8_t endPageAddress;
    uint8_t verticalScrollingOffset;
    bool leftElseRight;
    bool horizontalElseVertical;
    ScrollSetup(
        ScrollType type,
        uint8_t startPageAddress, uint8_t timeIntervalScrollStep,
        uint8_t endPageAddress, uint8_t verticalScrollingOffset,
        bool leftElseRight, bool horizontalElseVertical
    ) : type(type), 
        startPageAddress(startPageAddress), timeIntervalScrollStep(timeIntervalScrollStep),
        endPageAddress(endPageAddress), verticalScrollingOffset(verticalScrollingOffset),
        leftElseRight(leftElseRight), horizontalElseVertical(horizontalElseVertical) {}
   }