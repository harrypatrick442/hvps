#include "TemperatureMonitor.hpp"
#include "System/Aborter.hpp"
#include "Macros/GetFileName.hpp"
#include "esp_err.h"
const uint32_t TemperatureMonitor::UPDATE_INTERVAL_MS = 2000;
const char* TemperatureMonitor::getTag() {return GET_FILE_NAME;}

TemperatureMonitor& TemperatureMonitor::initializeWithParams(
	std::initializer_list<TemperatureSensorAndLimit> teperatureSensorAndLimit_s){
	std::vector<TemperatureSensorAndLimit> vec(teperatureSensorAndLimit_s.begin(), teperatureSensorAndLimit_s.end());
	return TemperatureMonitor::initialize(vec);
}
TemperatureMonitor::TemperatureMonitor(
	std::vector<TemperatureSensorAndLimit> teperatureSensorAndLimit_s)
	: _temperatureSensorAndLimit_s(teperatureSensorAndLimit_s.begin(), teperatureSensorAndLimit_s.end()),
	_timer(
		UPDATE_INTERVAL_MS,
		[this](){
			monitor();
		},
		true
	),
	_latestTemperatures(nullptr)
{
	size_t latestTemperaturesSize = _temperatureSensorAndLimit_s.size();
	_latestTemperatures = new float[latestTemperaturesSize];
	for(size_t i=0; i<latestTemperaturesSize; i++){
		_latestTemperatures[i]=-1;
	}
	_timer.start();
}
float TemperatureMonitor::getTemperature(const ITemperatureSensor& temperatureSensor)
{
	size_t index;
	if(!findSensorIndex(temperatureSensor, index)){
		SAFE_ABORT(
			"Temperature requested for unknown sensor %s",
			temperatureSensor.getName());
	}
	return _latestTemperatures[index];
}
bool TemperatureMonitor::findSensorIndex(
    const ITemperatureSensor& temperatureSensor, size_t& index) const
{
    for (size_t i = 0; i < _temperatureSensorAndLimit_s.size(); ++i) {
        if (&_temperatureSensorAndLimit_s[i].temperatureSensor == &temperatureSensor) {
            index =  i;
			return true;
        }
    }
	return false;
}
void TemperatureMonitor::monitor(){
	return;
	size_t index = 0;
	for (auto& temperatureSensorAndLimit : _temperatureSensorAndLimit_s) {
		ITemperatureSensor& temperatureSensor = temperatureSensorAndLimit.temperatureSensor;
		float temperature;
		esp_err_t err = temperatureSensor.getTemperature(temperature);
		if(err!=ESP_OK){
			//One more try
			err = temperatureSensor.getTemperature(temperature);
			if(err!=ESP_OK){
				SAFE_ABORT("Failed to read temperature sensor %s", temperatureSensor.getName());
				return;
			}
		}
		if(temperature>temperatureSensorAndLimit.limitDegreesC){
			SAFE_ABORT("Temperature on sensor %s exceeded limit of %f°C", temperatureSensor.getName(), temperatureSensorAndLimit.limitDegreesC);
			return;
		}
		_latestTemperatures[index++] = temperature;
	}
};