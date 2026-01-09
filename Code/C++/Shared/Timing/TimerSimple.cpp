#include "TimerSimple.hpp"
#include "System/SafeAbort.hpp"
#include "../Tasks/TaskFactory.hpp"
#include "Delay.hpp"
#include "../Core/UsageGuard.hpp"
const char* TimerSimple::TAG = "TimerSimple";
TimerSimple::TimerSimple(uint32_t intervalMs, Callback callback, bool repeat)
    : _intervalMs(intervalMs), _callback(callback), 
      _repeat(repeat) {
	if(callback==nullptr){
		SAFE_ABORT("Callback was null");
	}
}
TimerSimple::~TimerSimple() {
	
}
bool TimerSimple::start(){
	
	bool wasStarted = _started.exchange(true, std::memory_order_acq_rel);
	if (wasStarted) {
		return true;
	} 
	_taskHandle = nullptr;
    this->addRef(); // <-- task holds a ref from now on
    TaskFactory::createNonPriorityTask([this](){
		timerTask();
	}, "timerTask", &_taskHandle);
	if(_taskHandle!=nullptr){
		this->_isRunning.store(true, std::memory_order_release);
		return true;
	}
    // failed to create task: drop the task ref and reset started flag
    this->release();
	this->_started.store(false, std::memory_order_release);
	return false;
}
bool TimerSimple::getIsRunning() const {
    return _isRunning.load(std::memory_order_acquire);
}
void TimerSimple::dispose(){
	this->_disposed.store(true, std::memory_order_release);
}
void TimerSimple::timerTask() {
	for (;;) {
		Delay::ms(_intervalMs);
		if (_disposed.load(std::memory_order_acquire)) 
			break;
		_callback();
		if (!_repeat) 
			break;
	}
	_isRunning.store(false, std::memory_order_release);
    release();
}
