#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

class LockedFPGAInterface
{
public:
    LockedFPGAInterface(
        std::function<bool(size_t)>     getBit,
        std::function<uint8_t(size_t)>  getByte,
        std::function<uint16_t(size_t)> getUInt16);

    bool     getBit    (size_t index)     const;
    uint8_t  getByte   (size_t indexFrom) const;
    uint16_t getUInt16 (size_t indexFrom) const;

private:
    std::function<bool(size_t)>     _getBit;
    std::function<uint8_t(size_t)>  _getByte;
    std::function<uint16_t(size_t)> _getUInt16;
};
