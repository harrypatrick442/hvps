#pragma once
#include "Timing/FrequencyMeter.hpp"
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "../ControllerCore/HighSpeedCore.hpp"
#include "Temperature/TemperatureMonitor.hpp"
#include "../Ports/Port_ControllingMachine.hpp"
#include "Components/TemperatureSensors/Interfaces/ITemperatureSensor.hpp"
class LiveDataBroadcaster final
	:
	public SingletonBase<LiveDataBroadcaster>{
	public:
		static const char* getTag();
		using Callback = std::function<void()>;
		void start();
		void stop();
	private:
		friend class SingletonBase<LiveDataBroadcaster>;
		Port_ControllingMachine& _portControllingMachine;
		HighSpeedCore& _highSpeedCore;
		TemperatureMonitor& _temperatureMonitor;
		ITemperatureSensor& _mosfetTemperatureSensor;
		Timer _timer;
		EventConnection _eventConnectionPortOnOpen;
		EventConnection _eventConnectionPortOnClose;
		LiveDataBroadcaster(
			Port_ControllingMachine& port_ControllingMachine,
			HighSpeedCore& highSpeedCore, 
			TemperatureMonitor& temperatureMonitor,
			ITemperatureSensor& mosfetTemperatureSensor
		) noexcept;
		void _run();
	
};