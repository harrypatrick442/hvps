idf_component_register(
    SRCS 
	"VoltageFeedbackModuleBase.cpp"
	"ThresholdMonitor.cpp"
    "Ports/Port_FiberOpticChannel1.cpp"
	"IO/FiberOpticDuplexChannel_1.cpp"
	"IO/ADCChannels.cpp"
	"IO/Outputs.cpp"
	"IO/Inputs.cpp"
    PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
    INCLUDE_DIRS
        .       # lets you #include "Core/Event.hpp" or "Utils/Foo.hpp"
    REQUIRES
		Shared espcoredump
)
