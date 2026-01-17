#pragma once
#include "IGetTemperature.hpp"
#include "../../Interfaces/IGetName.hpp"
class ITemperatureSensor:public IGetTemperature, public IGetName{ 
};