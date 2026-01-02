#pragma once
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "Interfaces/IVoltageSource.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
class VoltageBroadcaster final
	:
	public SingletonBase<VoltageBroadcaster>{
	public:
		static const char* getTag();
	private:
		friend class SingletonBase<VoltageBroadcaster>;
		IVoltageSource& _voltageSource;
		Port_FiberOpticChannel1& _port;
		Timer _timer;
		VoltageBroadcaster(IVoltageSource& voltageSource, Port_FiberOpticChannel1& port) noexcept;
		void _run();
	
};