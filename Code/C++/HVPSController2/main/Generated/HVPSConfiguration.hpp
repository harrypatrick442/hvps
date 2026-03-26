#pragma once
#include <cstdint>
struct HVPSConfiguration {
    const float broadcastFrequencyHz;
    const float primaryCurrentFromRaw;
    const float firstStageVoltageFromRaw;
    const float outputVoltageFromRaw;
    const float firstStageVoltageThresholdVolts;
    const float maxAverageOutputPowerWatts;
    const float maxFlybackEnergyPerCycleJouls;
    const float maxOutputVoltageThresholdVolts;
    const float maxTemperatureMosfetDegreesC;
    const float minOutputVoltageThresholdVolts;
    const float vPsOverVadcRatio;
    const float villardCapacitorCapacitanceFarads;
    const uint8_t nVillardStages;
    const uint32_t pingTimeoutMilliseconds;
    const uint32_t sendPingIntervalMilliseconds;
    const uint32_t villardCapacitorsBleedTimeConstantSeconds;
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
