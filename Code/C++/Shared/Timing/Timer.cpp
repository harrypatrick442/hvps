#include "Timer.hpp"
#include "System/SafeAbort.hpp"
const char* Timer::TAG = "Timer";
Timer::Timer(uint32_t intervalMs, Callback callback, bool repeat)
    : _intervalMs(intervalMs), _callback(callback), 
      _repeat(repeat), _current(nullptr) {
	if(callback==nullptr){
		SAFE_ABORT("Callback was null");
	}
}

Timer::~Timer() {
	stop();
}

void Timer::setIntervalMs(uint32_t value){
    _intervalMs.store(value, std::memory_order_relaxed);
}
bool Timer::start() {
    const uint32_t interval = _intervalMs.load(std::memory_order_relaxed);
    std::unique_lock<std::mutex> lock(_mutex);
    if (_current) return true;

    auto* t = new TimerSimple(interval, _callback, _repeat);
    if (!t->start()) {
        // task failed to start; drop our only ref
        t->release();
        return false;
    }
    // success — keep a ref while “current”
    _current = t;
	return true;
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