idf_component_register(
    SRCS 
        "OutputVoltageFeedbackModule.cpp"
    PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
    INCLUDE_DIRS ""
    REQUIRES
		Shared
        VoltageFeedbackModuleBase
)
