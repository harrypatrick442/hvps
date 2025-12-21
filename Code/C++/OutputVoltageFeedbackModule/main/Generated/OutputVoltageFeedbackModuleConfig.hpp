#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr VoltageFeedbackModuleConfiguration Config1{
    .broadcastFrequencyHz = 2,
    .defaultThreshold = 80000,
    .vHvOverVadcRatio = 30800
};
inline VoltageFeedbackModuleConfiguration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
inline bool validateConfig(){
    uint32_t podConfig1 = Crc32::computePod(Config1);
    uint32_t podConfig2 = Crc32::computePod(Config2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for Config2 did not match the CRC32 for Config1");
        return false;
    }
    if (podConfig1 != 859879944){
        SAFE_ABORT("The CRC32 computed for Config1 did not match the expected value");
        return false;
    }
    return true;
}
