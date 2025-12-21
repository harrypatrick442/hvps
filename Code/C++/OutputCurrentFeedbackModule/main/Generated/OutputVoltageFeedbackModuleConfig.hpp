#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Configuration Config1{
    .vHvOverVadcRatio = 30800,
    .defaultThreshold = 80000,
    .broadcastFrequencyHz = 2
};
inline const uint32_t CONFIG_CRC32_EXPECTED = 3117605019;
inline Configuration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
bool validateConfiguration(){
	uint32_t podConfig1 = Crc32::computePod(Config1);
	uint32_t podConfig2 = Crc32::computePod(Config2);
	if(podConfig1 != podConfig2){
		SAFE_ABORT("The CRC32 computed for Config2 did not match the CRC32 for Config1");
		return false;
	}
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
		SAFE_ABORT("The CRC32 computed for Config1 did not match the expected value");
		return false;
	};
    return true;
}