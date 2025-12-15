#pragma once
#include <cstdint>
struct Configuration {
    const uint64_t onTimeMicroSeconds;
    const uint64_t offTimeMicroSeconds;
    const double maxOutputVoltageThreshold;
    const double minOutputVoltageThreshold;
    const double firstStageVoltageThreshold;
    const double maxAverageOutputPower;
    const uint8_t nStages;
    const double broadcastFrequencyHz;
    const double vPsOverVadcRatio;
    const uint64_t pingTimeoutMilliseconds;
};
