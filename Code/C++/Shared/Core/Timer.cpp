#include "Timer.hpp"
#include "../System/Aborter.hpp"
const char* Timer::TAG = "Timer";
Timer::Timer(uint32_t intervalMs, Callback callback, bool repeat)
    : _intervalMs(intervalMs), _callback(callback), 
      _repeat(repeat), _current(nullptr) {
	if(callback==nullptr){
		Aborter::safeAbort(TAG, "Callback was null");
	}
}

Timer::~Timer() {
	stop();
}

void Timer::start() {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_current) return;

    auto* t = new TimerSimple(_intervalMs, _callback, _repeat);
    if (!t->start()) {
        // task failed to start; drop our only ref
        t->release();
        return;
    }
    // success — keep a ref while “current”
    _current = t;
}

void Timer::stop() {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_current==nullptr) return;
	_current->dispose();
	_current->release();
	_current = nullptr;
}

bool Timer::isRunning() const {
    std::unique_lock<std::mutex> lock(_mutex);
    return _current!=nullptr&&_current->getIsRunning();
}