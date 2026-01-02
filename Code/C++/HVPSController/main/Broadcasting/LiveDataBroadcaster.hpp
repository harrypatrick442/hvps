#pragma once
#include "Timing/FrequencyMeter.hpp"
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "../ControllerCore/LiveDataCache.hpp"
#include "../Ports/Port_ControllingMachine.hpp"
#include "Structs/VoltageWithRawAndTime.hpp"
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
		LiveDataCache& _liveDataCache;
		Port_ControllingMachine& _portControllingMachine;
		FrequencyMeter& _frequencyMeter;
		Timer _timer;
		EventConnection _eventConnectionPortOnOpen;
		EventConnection _eventConnectionPortOnClose;
		LiveDataBroadcaster(LiveDataCache& liveDataCache, Port_ControllingMachine& port_ControllingMachine,
			FrequencyMeter& frequencyMeter) noexcept;
		void _run();
	
};