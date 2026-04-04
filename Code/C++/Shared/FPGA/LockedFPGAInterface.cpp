#include "LockedFPGAInterface.hpp"

LockedFPGAInterface::LockedFPGAInterface(
    std::function<bool(size_t)>     getBit,
    std::function<uint8_t(size_t)>  getByte,
    std::function<uint16_t(size_t)> getUInt16)
    : _getBit    (getBit)
    , _getByte   (getByte)
    , _getUInt16 (getUInt16)
{
}

bool LockedFPGAInterface::getBit(size_t index) const
{
    return _getBit(index);
}

uint8_t LockedFPGAInterface::getByte(size_t indexFrom) const
{
    return _getByte(indexFrom);
}

uint16_t LockedFPGAInterface::getUInt16(size_t indexFrom) const
{
    return _getUInt16(indexFrom);
}
