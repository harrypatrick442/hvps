#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/Aborter.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Configuration Config1{
    .broadcastFrequencyHz = 2,
    .defaultThreshold = 14255.604,
    .vHvOverVadcRatio = 5600
};
inline const uint32_t CONFIG_CRC32_EXPECTED = 1513659569;
inline Configuration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
inline bool validateConfiguration(){
    uint32_t podConfig1 = Crc32::computePod(Config1);
    uint32_t podConfig2 = Crc32::computePod(Config2);
    if (podConfig1 != podConfig2){
        Aborter::safeAbort("Configuration", "The CRC32 computed for Config2 did not match the CRC32 for Config1");
        return false;
    }
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
        Aborter::safeAbort("Configuration", "The CRC32 computed for Config1 did not match the expected value");
        return false;
    }
    return true;
}
