#pragma once
struct HVPSConfig {
  uint64_t OnTimeMicroSeconds;
  uint64_t OffTimeMicroSeconds;
  double SafeOutputVoltage;
  uint8_t NStages;
};

	const uint64_t ON_TIME_US = 62;
	const uint64_t ON_TIME_US_2 = 62;
	const uint64_t OFF_TIME_US = 62;
	const uint64_t OFF_TIME_US_2 = 62;
	const double SAFE_OUTPUT_VOLTAGE = 30;
inline constexpr HVPSConfig Config1{
  OnTimeMicroSeconds=62, OffTimeMicroSeconds=62, SafeOutputVoltage=0.3, NStages=8
};
/*
static_assert(RigA.stages > 0, "stages must be > 0");
static_assert(RigA.maxKv >= RigA.targetKv, "maxKv must be ≥ targetKv");
static_assert(RigA.safeKv <= RigA.targetKv, "safeKv must be ≤ targetKv");*/