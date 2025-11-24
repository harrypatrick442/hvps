#pragma once
#include <cstdint>
struct Configuration {
    const uint32_t idleColour;
    const uint32_t liveColour;
    const uint32_t runningSystemChecksColour;
    const uint32_t shuttingDownColour;
    const uint32_t shutDownColour;
    const uint32_t errorColour;
    const uint32_t unknownColour;
};
