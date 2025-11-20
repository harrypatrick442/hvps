#include "ADC/ADC.hpp"
#include "Broadcasting/LiveDataBroadcaster.hpp"
#include "Communication/Bluetooth/Bluetooth_BR_EDR.hpp"
#include "Communication/I2C/I2C.hpp"
#include "Communication/I2C/I2CConfiguration.hpp"
#include "IO/SoftStartHandler.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Storage/Flash.hpp"
#include "Ports/Port_ControllingMachine.hpp"
#include "Ports/Port_FirstStageVoltageFeedback.hpp"
#include "Ports/Port_OutputVoltageFeedback.hpp"
#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Generated/HVPSConfig.hpp"
#include "Timing/Delay.hpp"
#include "System/CrashReporter.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "System/SubsystemIdentifier.hpp"

#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

static const char *TAG = "HVPS";

extern "C" void app_main(void)
{
	SubsystemIdentifier::set(SubsystemIdentifiers::HVPSController);
	Aborter::setToSafe(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	Delay::ms(10000);
	Log::Info(TAG, "Starting HVPSController....");
	Delay::ms(1000);
    Flash::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGI(TAG, "Starting HVPSController...");
    StayTheFuckAwake::initialize();
	validateConfiguration();
	ADC::initialize();
	SoftStartHandler::doSoftStart(Config1, Config2);
	Log::Info(TAG, "Did soft start");
	Inputs::initialize();
	Log::Info(TAG, "Initialized inputs");
    // Initialize the I2C bus
	//I2CConfiguration i2cConfiguration;//Default
    //I2C::initialize(i2cConfiguration);
    //I2C& i2c = I2C::getInstance();
    WatchdogFeeder
        ::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
		
	Port_FirstStageVoltageFeedback& port_FirstStageVoltageFeedback = Port_FirstStageVoltageFeedback::initialize();
	Port_OutputVoltageFeedback& port_OutputVoltageFeedback = Port_OutputVoltageFeedback::initialize();
    Bluetooth::initialize(
        "HVPS", 
        "HVPSControllerServer"
     );
    Bluetooth& bluetooth = Bluetooth::getInstance();
	LiveDataCache& liveDataCache = LiveDataCache::initialize(
		port_FirstStageVoltageFeedback,
		port_OutputVoltageFeedback
	);
	bool inError = Aborter::hasLastAbortReason()||CrashReporter::hasCoreDumpSummary();
	HighSpeedCore& highSpeedCore = HighSpeedCore::initialize(
		port_FirstStageVoltageFeedback,
		port_OutputVoltageFeedback,
		liveDataCache,
		inError
	);
	Port_ControllingMachine& portControllingMachine 
		= Port_ControllingMachine::initialize(
			bluetooth, 
			highSpeedCore,
			Config1.pingTimeoutMilliseconds,
			port_FirstStageVoltageFeedback,
			port_OutputVoltageFeedback
	);
							 
	LiveDataBroadcaster::initialize(liveDataCache, portControllingMachine);
	vTaskDelete(NULL); // Delete the current task*/
}  

