#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <cstdint>
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Watchdog.hpp"
#include "WatchdogsAndSize.hpp"
class WatchdogCollection final:
	public SingletonBase<WatchdogCollection>{
public:
	static const float PERIOD_MS;
	static const char* getTag();
	DISALLOW_COPY_MOVE(WatchdogCollection);
private:
	friend class SingletonBase<WatchdogCollection>;
	std::vector<std::shared_ptr<Watchdog>> _watchdogs;
	WatchdogsAndSize* _watchdogsAndSize;
	std::mutex _mutexAdd;
	Timer _timer;
public:
	WatchdogCollection();
	std::shared_ptr<Watchdog> add(const char* name, float hz);
private:
	void updateRawPointerBuffer_Unlocked();
	void check();
	static bool IRAM_ATTR checkTrampoline(void* arg);
};