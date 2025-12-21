#include "MRTExample.hpp"
#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "System/SafeAbort.hpp"
#include "IO/Outputs.hpp"
#include "Storage/Flash.hpp"
#include "Timing/Delay.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "Enums/SystemState.hpp"
#include "esp_log.h"
#include "Generated/Peripheral1Config.hpp"
#include "IO/PinDefinitions.hpp"
#include "Timing/Delay.hpp"
#include "Communication/MRT/MRTChannel.hpp"
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

extern "C" void app_main(void)
{
	Log::Info("MRTExample", "A");
	Aborter::setToSafe(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
    Flash::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	MRTChannel channel(
		PinDefinitions::TX,
		PinDefinitions::RX,
		400,
		false,
		true
	);
	channel.printTimingConfig();
	channel.configure();
	bool b = false;
	const char* chars = "abcde";
	const char* chars2 = "fghij";
	char* received = new char[101];
	while(true){
		//Log::Info("MRTExample", "D");
		size_t nWrote = channel.writeBytes(b?chars:chars2, 5);
		b=!b;
		//Log::Info("MRTExample", "wrote %zu", nWrote);
		size_t nRead = channel.readBytes(received, 100, 0);
		received[nRead] = '\0';
		//Log::Info("MRTExample", "read %zu", nRead);
		Log::Info("MRTExample", "read %s", received);
	}
	vTaskDelete(NULL); // Delete the current task
}
