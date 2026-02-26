#include "LowerSnubberDiodeTemperatureSensor.hpp"
#include "../Peripherals/Addresses.hpp"

LowerSnubberDiodeTemperatureSensor::LowerSnubberDiodeTemperatureSensor(
          IReadWrite& readWrite)
    : TemperatureSensor(
          readWrite,
          Addresses::LOWER_SNUBBER_DIODE_TEMPERATURE_SENSOR,
          TAG)
{
}
