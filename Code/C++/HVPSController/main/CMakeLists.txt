idf_component_register(
	SRCS 
		"HVPSController.cpp"
		"Guids/BluetoothUUIDHelper.cpp"
		"IO/ADCChannels.cpp"
		"IO/FirstStageVoltageFeedbackFiberOpticDuplexChannel.cpp"
		"IO/Inputs.cpp"
		"IO/Outputs.cpp"
		"IO/OutputVoltageFeedbackFiberOpticDuplexChannel.cpp"
		"IO/OtherPeripheralsFiberOpticDuplexChannel.cpp"
		"IO/PinDefinitions.cpp"
		"Ports/Port_ControllingMachine.cpp"
		"Ports/Port_FirstStageVoltageFeedback.cpp"
		"Ports/Port_OutputVoltageFeedback.cpp"
		"Ports/Port_VoltageFeedbackBase.cpp"
		"Ports/Port_OtherPeripherals.cpp"
		"ControllerCore/HighSpeedCore.cpp"
		"ControllerCore/LiveDataCache.cpp"
		"ControllerCore/SystemChecks.cpp"
		"ControllerCore/SystemChecksResult.cpp"
		"Broadcasting/LiveDataBroadcaster.cpp"
		"IO/SoftStartHandler.cpp"
		"IO/OutputVoltageFeedbackFiberOpticDuplexChannel.cpp"
		"IO/FirstStageVoltageFeedbackFiberOpticDuplexChannel.cpp"
	PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
	INCLUDE_DIRS ""
	REQUIRES
		Shared espcoredump
)