/* 
* IGetTemperatureInfo.h
*
* Created: 09/10/2017 13:01:34
* Author: SoftwareEngineer
*/


#ifndef I_TEMPERATURE_SENSOR_H
#define I_TEMPERATURE_SENSOR_H
#include "IGetTemperature.hpp"
#include "../../Interfaces/IGetName.hpp"
class ITemperatureSensor:public IGetTemperature, public IGetName{ 
};
#endif //I_TEMPERATURE_SENSOR_H