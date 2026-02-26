#include "MOSFETTemperatureSensor.hpp"
#include "../Peripherals/Addresses.hpp"

MOSFETTemperatureSensor::MOSFETTemperatureSensor(
          IReadWrite& readWrite)
    : TemperatureSensor(
          readWrite,
          Addresses::MOSFET_TEMPERATURE_SENSOR,
          TAG)
{
}
