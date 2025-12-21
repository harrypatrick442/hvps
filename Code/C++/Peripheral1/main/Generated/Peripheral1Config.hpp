#pragma once
#include "Peripheral1Configuration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Peripheral1Configuration Config1{
    .idleColour = 14724644,
    .liveColour = 255,
    .runningSystemChecksColour = 5631,
    .shuttingDownColour = 16523,
    .shutDownColour = 65280,
    .errorColour = 6227959,
    .unknownColour = 16777215,
    .idleFlashDelayMs = 250,
    .liveFlashDelayMs = 250,
    .runningSystemChecksFlashDelayMs = 500,
    .shuttingDownFlashDelayMs = 500,
    .shutDownFlashDelayMs = 0,
    .errorFlashDelayMs = 500,
    .unknownFlashDelayMs = 500
};
inline Peripheral1Configuration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
inline bool validateConfig(){
    uint32_t podConfig1 = Crc32::computePod(Config1);
    uint32_t podConfig2 = Crc32::computePod(Config2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for Config2 did not match the CRC32 for Config1");
        return false;
    }
    if (podConfig1 != 712775843){
        SAFE_ABORT("The CRC32 computed for Config1 did not match the expected value");
        return false;
    }
    return true;
}
