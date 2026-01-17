#include "WatchdogCollection.hpp"
#include "System/Aborter.hpp"
const float WatchdogCollection::PERIOD_US = 100f;
WatchdogCollection::WatchdogCollection():
	_watchdogs(),
	_mutexAdd(),
	_interuptTimer(TIMER_GROUP_0, TIMER_0, PERIOD, ESP_INTR_FLAG_LEVEL3, true){
	esp_err_t err = _interuptTimer.configure(&WatchdogCollection::checkTrampoline, this);
	if(err!=ESP_OK)
	{
		SAFE_ABORT("Failed to initialize interupt timer with error %s",  esp_err_to_name(err));
	}
}
std::shared_ptr<Watchdog> WatchdogCollection::add(const char* name, float hz){
	float interuptHz = 1000000f/PERIOD_US;
	float countFrom = std::ceil(interuptHz / hz);
	if(countFrom<=0)
		countFrom = 1;
	auto shared = std::make_shared<Watchdog>(name, countFrom);
	_mutexAdd.lock();
	_watchdogsShared.push_back(shared);
	updateRawPointerBuffer_Unlocked();
	_mutexAdd.unlock();
	return shared;
}
void WatchdogCollection::updateRawPointerBuffer_Unlocked(){
	WatchdogsAndSize* newWatchdogsAndSize = new WatchdogsAndSize(_watchdogsShared);
	WatchdogsAndSize* existing =  _watchdogsAndSize;
	_watchdogsAndSize = newWatchdogsAndSize;
	delete existing;
}
void WatchdogCollection::check(){
	WatchdogsAndSize* watchdogsAndSize = _watchdogsAndSize;
	for(size_t i = 0; i<watchdogsAndSize.size; i++){
		Watchdog* watchdog = watchdogsAndSize.entries[i];
		if(watchdog->check()){
			continue;
		}
		SAFE_ABORT("Watchdog timeout for watchdog %s", watchdog->getName());
	}
}}
void IRAM_ATTR WatchdogCollection::checkTrampoline(void* arg) {
    static_cast<WatchdogCollection*>(arg)->check();
}