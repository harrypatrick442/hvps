idf_component_register(
    SRCS 
	"MRTExample.cpp"
	"IO/FiberOpticDuplexChannel_1.cpp"
	"IO/Outputs.cpp"
	"IO/PinDefinitions.cpp"
    PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
    INCLUDE_DIRS
        .       # lets you #include "Core/Event.hpp" or "Utils/Foo.hpp"
    REQUIRES
		Shared espcoredump led_strip
)
