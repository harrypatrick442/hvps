
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ADC/ADCOneShot.hpp"
#include "Broadcasting/LiveDataBroadcaster.hpp"
#include "Communication/Bluetooth/Bluetooth_BR_EDR.hpp"
#include "IO/PowerConditioningMonitor.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Storage/Flash.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Ports/Port_ControllingMachine.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Generated/HVPSConfig.hpp"
#include "Timing/Delay.hpp"
#include "System/CrashReporter.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "System/SubsystemIdentifier.hpp"
#include "IO/IOInteruptHelper.hpp"
#include "System/SafeAbort.hpp"
#include "Communication/I2C/I2C.hpp"
#include "Communication/I2C/I2CConfiguration.hpp"
#include "Temperature/MOSFETTemperatureSensor.hpp"
#include "Temperature/TemperatureSensorAndLimit.hpp"
#include "Temperature/TemperatureMonitor.hpp"
#include "IO/PinDefinitions.hpp"
#include "Peripherals/LocalUI.hpp"
#include "Enums/SystemState.hpp"
#include "Graphics/HVPSLEDDisplay.hpp"
#include "Watchdog/WatchdogCollection.hpp"
#include "IO/HVPSFPGABus.hpp"
#include "ControllerCore/ThresholdController.hpp"
#include <driver/gpio.h>
#include "Generated/HVPS_FPGAInterface.hpp"

static const char *TAG = "HVPS";

extern "C" void app_main(void)
{
	SubsystemIdentifier::set(SubsystemIdentifiers::HVPSController);
	Aborter::initialize(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	IOInteruptHelper::installISRHandlerIfNotAlready();
	Delay::ms(1000);
    Flash::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
	LOG_INFO("1 to ticks is %f: ", static_cast<float>(pdMS_TO_TICKS(1)));
	
    StayTheFuckAwake::disableSleepSources();
    StayTheFuckAwake::disablePowerManagement();
    StayTheFuckAwake::disableWiFiPowerSave();
    StayTheFuckAwake::disableWatchdog();
	
	validateConfig();
	HVPSLEDDisplay& hVPSLEDDisplay 
		= HVPSLEDDisplay::initialize(Config1);
	WatchdogCollection::initialize();
	ADCOneShot::initialize();
	PowerConditioningMonitor& powerConditioningMonitor = PowerConditioningMonitor::initialize(Config1, Config2);
	powerConditioningMonitor.waitForSoftStart();
	Inputs::initialize();
	HVPSFPGABus& hvpsFPGABus = HVPSFPGABus::initialize();
	
	
	
	
    // Initialize the I2C bus
	I2CConfigurationOptions options;
	options.sclPin = static_cast<gpio_num_t>(PinDefinitions::I2C_SCL_PIN);
	options.sdaPin = static_cast<gpio_num_t>(PinDefinitions::I2C_SDA_PIN);
	I2CConfiguration i2cConfiguration(options);//Default
    I2C::initialize(i2cConfiguration);
    I2C& i2c = I2C::getInstance();
	MOSFETTemperatureSensor& mosfetTemperatureSensor = MOSFETTemperatureSensor::initialize(i2c);
	LOG_INFO("Doing bluetooth");
    Bluetooth::initialize(
        "HVPS2", 
        "HVPSControllerServer"
     );
    Bluetooth& bluetooth = Bluetooth::getInstance();
	bool inError = Aborter::hasLastAbortReason()||CrashReporter::hasCoreDumpSummary();
	
	HVPS_FPGAInterface& fpgaInterface = HVPS_FPGAInterface::initialize(hvpsFPGABus);
	ThresholdController& thresholdController = ThresholdController::initialize(Config1, Config2, fpgaInterface);
	HighSpeedCore& highSpeedCore = HighSpeedCore::initialize(
		Config1,
		Config2,
		fpgaInterface,
		inError
	);
	LocalUI& localUI = LocalUI::initialize(highSpeedCore, hVPSLEDDisplay);
	Port_ControllingMachine& portControllingMachine 
		= Port_ControllingMachine::initialize(
			bluetooth, 
			highSpeedCore,
			Config1.sendPingIntervalMilliseconds,
			Config1.pingTimeoutMilliseconds
	);
	TemperatureSensorAndLimit mosfetTemperatureSensorAndLimit(
		mosfetTemperatureSensor, Config1.maxTemperatureMosfetDegreesC);
	TemperatureMonitor& temperatureMonitor = TemperatureMonitor::initializeWithParams(
	{
		mosfetTemperatureSensorAndLimit
	});
	
	LiveDataBroadcaster::initialize(
		portControllingMachine,
		highSpeedCore,
		temperatureMonitor,
		mosfetTemperatureSensor
	);
	LOG_INFO("Initialized");
	/*
	while(true){
		hVPSLEDDisplay.indicateState(SystemState::Error);
		
		//hvpsFPGABus.setGoLive(false);
		//hvpsFPGABus.setInShift(false);
		//hvpsFPGABus.setOutShift(false);
		//hvpsFPGABus.setToOutput(false);
		//hvpsFPGABus.setInValue(false);
		Delay::ms(1000);
	}*/
	vTaskDelete(NULL); // Delete the current task*/
}  

