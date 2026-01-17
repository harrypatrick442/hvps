#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Temperature/TemperatureSensorAndLimit.hpp"
#include "Components/TemperatureSensors/Interfaces/ITemperatureSensor.hpp"
#include "Timing/Timer.hpp"
#include <initializer_list>
#include <functional>   // reference_wrapper
#include <vector>
#include <cstdint>
class TemperatureMonitor final: public SingletonBase<TemperatureMonitor>{

private:
	static const uint32_t UPDATE_INTERVAL_MS;
    std::vector<TemperatureSensorAndLimit> _temperatureSensorAndLimit_s;
	Timer _timer;
	float* _latestTemperatures;
	DISALLOW_COPY_MOVE(TemperatureMonitor);
public:
	static const char* getTag();
	static TemperatureMonitor& initializeWithParams(
		std::initializer_list<TemperatureSensorAndLimit> teperatureSensorAndLimit_s);
	float getTemperature(const ITemperatureSensor& temperatureSensor);
private:
    friend class SingletonBase<TemperatureMonitor>;
	TemperatureMonitor(
		std::vector<TemperatureSensorAndLimit> teperatureSensorAndLimit_s);
	bool findSensorIndex(const ITemperatureSensor& temperatureSensor, size_t& index) const;
private:
	void monitor();
};