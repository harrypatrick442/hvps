#ifndef I_GET_TEMPERATURE_H
#define I_GET_TEMPERATURE_H
#include <cstdint>
class IGetTemperature{
	public: virtual esp_err_t LM75A::GetTemperature(float& temperature)=0;
};
#endif //I_GET_TEMPERATURE_H