#pragma once
#include "Watchdog.hpp"
#include "Timing/InteruptTimer.hpp"
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
class WatchdogCollection{
public:
	static const float PERIOD_US;
private:
	std::vector<std::shared_ptr<Watchdog>> _watchdogs;
	std::mutex _mutexAdd;
	InteruptTimer _interuptTimer;
public:
	WatchdogCollection(std::initializer_list<Watchdog&> watchdogs);
	std::shared_ptr<Watchdog> add(const char* name, float hz);
private:
	void updateRawPointerBuffer_Unlocked();
	void check();
	static void IRAM_ATTR checkTrampoline(void* arg);
}