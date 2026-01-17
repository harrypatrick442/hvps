#include <cmath>
#include "WatchdogCollection.hpp"
#include "System/Aborter.hpp"
#include "Macros/GetFileName.hpp"
#include "esp_err.h"
const float WatchdogCollection::PERIOD_MS = 100.0f;
const char* WatchdogCollection::getTag() {return GET_FILE_NAME;}
WatchdogCollection::WatchdogCollection():
	_watchdogs(),
	_watchdogsAndSize(nullptr),
	_mutexAdd(),
	_timer(
		PERIOD_MS,
		[this](){
			check();
		},
		true
	){
	if(!_timer.start())
	{
		SAFE_ABORT("Failed to initialize timer");
	}
}
std::shared_ptr<Watchdog> WatchdogCollection::add(const char* name, float hz){
	float interuptHz = 1000.0f/PERIOD_MS;
	float countFrom = ceilf(interuptHz / hz);
	if(countFrom<=0)
		countFrom = 1;
	auto shared = std::make_shared<Watchdog>(name, countFrom);
	_mutexAdd.lock();
	_watchdogs.push_back(shared);
	updateRawPointerBuffer_Unlocked();
	_mutexAdd.unlock();
	return shared;
}
void WatchdogCollection::updateRawPointerBuffer_Unlocked(){
	WatchdogsAndSize* newWatchdogsAndSize = new WatchdogsAndSize(_watchdogs);
	WatchdogsAndSize* existing =  _watchdogsAndSize;
	_watchdogsAndSize = newWatchdogsAndSize;
	if(existing!=nullptr){
		delete existing;
	}
}
void WatchdogCollection::check(){
	WatchdogsAndSize* watchdogsAndSize = _watchdogsAndSize;
	if(watchdogsAndSize==nullptr){
		return;
	}
	for(size_t i = 0; i<watchdogsAndSize->size; i++){
		Watchdog* watchdog = watchdogsAndSize->entries[i];
		if(watchdog->check()){
			continue;
		}
		SAFE_ABORT("Watchdog timeout for watchdog %s", watchdog->getName());
	}
}
bool IRAM_ATTR WatchdogCollection::checkTrampoline(void* arg) {
    static_cast<WatchdogCollection*>(arg)->check();
	return false;
}