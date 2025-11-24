#pragma once
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "../ControllerCore/LiveDataCache.hpp"
#include "../Ports/Port_ControllingMachine.hpp"
class LiveDataBroadcaster final
	:
	public SingletonBase<LiveDataBroadcaster>{
	public:
		static inline constexpr const char* TAG = "LiveDataBroadcaster";
		using Callback = std::function<void()>;
		void start();
		void stop();
	private:
		friend class SingletonBase<LiveDataBroadcaster>;
		LiveDataCache& _liveDataCache;
		Port_ControllingMachine& _portControllingMachine;
		Timer _timer;
		LiveDataBroadcaster(LiveDataCache& liveDataCache, Port_ControllingMachine& port_ControllingMachine) noexcept;
		void _run();
	
};