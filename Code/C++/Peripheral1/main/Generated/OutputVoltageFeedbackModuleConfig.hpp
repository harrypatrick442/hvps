#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/Aborter.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Configuration OutputVoltageFeedbackModuleConfig1{
    .broadcastFrequencyHz = 2,
    .defaultThreshold = 80000,
    .vHvOverVadcRatio = 30800
};
inline const uint32_t CONFIG_CRC32_EXPECTED = 859879944;
inline Configuration OutputVoltageFeedbackModuleConfig2 = OutputVoltageFeedbackModuleConfig1;//This one is in RAM. OutputVoltageFeedbackModuleConfig1 is in ROM.
inline bool validateConfiguration(){
    uint32_t podConfig1 = Crc32::computePod(OutputVoltageFeedbackModuleConfig1);
    uint32_t podConfig2 = Crc32::computePod(OutputVoltageFeedbackModuleConfig2);
    if (podConfig1 != podConfig2){
        Aborter::safeAbort("Configuration", "The CRC32 computed for OutputVoltageFeedbackModuleConfig2 did not match the CRC32 for OutputVoltageFeedbackModuleConfig1");
        return false;
    }
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
        Aborter::safeAbort("Configuration", "The CRC32 computed for OutputVoltageFeedbackModuleConfig1 did not match the expected value");
        return false;
    }
    return true;
}
