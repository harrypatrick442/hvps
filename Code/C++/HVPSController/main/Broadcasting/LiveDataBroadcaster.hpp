#pragma once
#include "../Core/Timer.hpp"
#include "../Core/SingletonBase.hpp"
#include "../States/LiveDataCache.hpp"
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
		Timer _timer;
		LiveDataCache& _liveDataCachep;
		Port_ControllingMachine& _portControllingMachine;
		LiveDataBroadcaster(LiveDataCache& liveDataCache, Port_ControllingMachine& port_ControllingMachine) noexcept;
		void _run();
	
}