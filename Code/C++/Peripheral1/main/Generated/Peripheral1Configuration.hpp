#pragma once
#include <cstdint>
struct Peripheral1Configuration {
    const uint32_t idleColour;
    const uint32_t liveColour;
    const uint32_t runningSystemChecksColour;
    const uint32_t shuttingDownColour;
    const uint32_t shutDownColour;
    const uint32_t errorColour;
    const uint32_t unknownColour;
    const uint32_t idleFlashDelayMs;
    const uint32_t liveFlashDelayMs;
    const uint32_t runningSystemChecksFlashDelayMs;
    const uint32_t shuttingDownFlashDelayMs;
    const uint32_t shutDownFlashDelayMs;
    const uint32_t errorFlashDelayMs;
    const uint32_t unknownFlashDelayMs;
};
