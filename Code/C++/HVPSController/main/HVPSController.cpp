#include "ADC/ADC.hpp"
#include "Broadcasting/LiveDataBroadcaster.hpp"
#include "Communication/Bluetooth/Bluetooth_BR_EDR.hpp"
#include "IO/SoftStartHandler.hpp"
#include "IO/Inputs.hpp"
#include "IO/Outputs.hpp"
#include "Storage/Flash.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Ports/Port_ControllingMachine.hpp"
#include "Ports/Port_FirstStageVoltageFeedback.hpp"
#include "Ports/Port_OutputVoltageFeedback.hpp"
#include "Ports/Port_OtherPeripherals.hpp"
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
#include "Temperature/LowerSnubberDiodeTemperatureSensor.hpp"
#include "Temperature/MOSFETTemperatureSensor.hpp"
#include "Temperature/TemperatureSensorAndLimit.hpp"
#include "Temperature/TemperatureMonitor.hpp"
#include "IO/PinDefinitions.hpp"
#include "Watchdog/WatchdogCollection.hpp"
#include <driver/gpio.h>  // gpio_num_t, GPIO_NUM_*
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

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
	
    StayTheFuckAwake::disableSleepSources();
    StayTheFuckAwake::disablePowerManagement();
    StayTheFuckAwake::disableWiFiPowerSave();
    StayTheFuckAwake::disableWatchdog();
	
	validateConfig();
	WatchdogCollection::initialize();
	ADC::initialize();
	SoftStartHandler::doSoftStart(Config1, Config2);
	Inputs::initialize();
    // Initialize the I2C bus
	I2CConfigurationOptions options;
	options.sclPin = static_cast<gpio_num_t>(PinDefinitions::I2C_SCL_PIN);
	options.sdaPin = static_cast<gpio_num_t>(PinDefinitions::I2C_SDA_PIN);
	I2CConfiguration i2cConfiguration(options);//Default
    I2C::initialize(i2cConfiguration);
    I2C& i2c = I2C::getInstance();
	MOSFETTemperatureSensor& mosfetTemperatureSensor = MOSFETTemperatureSensor::initialize(i2c);
	LowerSnubberDiodeTemperatureSensor& lowerSnubberDiodeTemperatureSensor = LowerSnubberDiodeTemperatureSensor::initialize(i2c);
		
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
		Config1,
		Config2,
		port_FirstStageVoltageFeedback,
		port_OutputVoltageFeedback,
		liveDataCache,
		inError
	);
	Port_OtherPeripherals& port_OtherPeripherals = Port_OtherPeripherals::initialize(highSpeedCore);
	Port_ControllingMachine& portControllingMachine 
		= Port_ControllingMachine::initialize(
			bluetooth, 
			highSpeedCore,
			Config1.pingTimeoutMilliseconds,
			port_FirstStageVoltageFeedback,
			port_OutputVoltageFeedback
	);
	TemperatureSensorAndLimit mosfetTemperatureSensorAndLimit(mosfetTemperatureSensor, Config1.maxTemperatureMosfetDegreesC);
	TemperatureSensorAndLimit lowerSnubberDiodeTemperatureSensorAndLimit(lowerSnubberDiodeTemperatureSensor, Config1.maxTemperatureLowerSnubberDiodeDegreesC);
	TemperatureMonitor& temperatureMonitor = TemperatureMonitor::initializeWithParams(
	{
		mosfetTemperatureSensorAndLimit,
		lowerSnubberDiodeTemperatureSensorAndLimit
	});
	
	LiveDataBroadcaster::initialize(
		liveDataCache, 
		portControllingMachine,
		highSpeedCore,
		temperatureMonitor,
		mosfetTemperatureSensor,
		lowerSnubberDiodeTemperatureSensor
	);
	vTaskDelete(NULL); // Delete the current task*/
}  

