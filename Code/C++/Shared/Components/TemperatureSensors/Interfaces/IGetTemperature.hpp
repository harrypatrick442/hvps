#ifndef I_GET_TEMPERATURE_H
#define I_GET_TEMPERATURE_H
#include <cstdint>
class IGetTemperature{
	public: virtual uint8_t GetTemperature(bool& successful)=0;
};
#endif //I_GET_TEMPERATURE_H