idf_component_register(
	SRCS 
		"HVPSController.cpp"
		"Guids/BluetoothUUIDHelper.cpp"
		"IO/ADCChannels.cpp"
		"IO/Inputs.cpp"
		"IO/Outputs.cpp"
		"Peripherals/Addresses.cpp"
		"Ports/Port_ControllingMachine.cpp"
		"ControllerCore/HighSpeedCore.cpp"
		"ControllerCore/SystemChecks.cpp"
		"ControllerCore/SystemChecksResult.cpp"
		"Broadcasting/LiveDataBroadcaster.cpp"
		"IO/SoftStartHandler.cpp"
		"Temperature/MOSFETTemperatureSensor.cpp"
	PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
	INCLUDE_DIRS ""
	REQUIRES
		Shared espcoredump
)