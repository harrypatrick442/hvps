#include "TemperatureSensorAndLimit.hpp"
TemperatureSensorAndLimit::TemperatureSensorAndLimit(ITemperatureSensor& sensor,
                              float limitC)
        : temperatureSensor(sensor)
        , limitDegreesC(limitC)
{
	
}
