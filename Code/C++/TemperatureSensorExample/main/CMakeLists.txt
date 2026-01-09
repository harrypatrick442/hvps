idf_component_register(
    SRCS 
	"TemperatureSensorExample.cpp"
	"IO/PinDefinitions.cpp"
    PRIV_REQUIRES spi_flash driver bt nvs_flash esp_adc
    INCLUDE_DIRS
        .       # lets you #include "Core/Event.hpp" or "Utils/Foo.hpp"
    REQUIRES
		Shared espcoredump 
)
