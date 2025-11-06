#include "TransientWorker.hpp"
#include "../System/Aborter.hpp";
#include "TaskFactory.hpp";
TransientWorker::TransientWorker(
	UBaseType_t maxQueueLength,
	uint32_t idleTimeoutMs,
	bool abortOnQueueOverflow
)
    : 
	_idleTimeoutMs(idleTimeoutMs),
	_abortOnQueueOverflow(abortOnQueueOverflow),
	_queue(xQueueCreate(maxQueueLength, sizeof(Job*))),
	_mutex(xSemaphoreCreateMutex()),
	_alive(false),
	_idleTicks(pdMS_TO_TICKS(idleTimeoutMs)){
		if (!_queue || !_mutex) {
			Aborter::safeAbort(TAG, "Failed to initialize TransientWorker resources");
		}
	}

TransientWorker::~TransientWorker() {
    if (_queue){
		vQueueDelete(_queue);
	}
    if (_mutex) vSemaphoreDelete(_mutex);
}

bool TransientWorker::enqueue(Job job) {
	Job* j = new (std::nothrow) Job(std::move(job));
    if (xQueueSend(_queue, &j, 0) != pdPASS) {
		if(_abortOnQueueOverflow){
			Aborter::safeAbort(TAG, "Queue overflowed");
			return false;
		}
        Log::Warn(TAG, "TransientWorker queue full; dropping job.");
        delete j;
        return false;
    }
	takeSemaphore();
    if (_alive) {
		giveSemaphore();
		return true;
	}
	_alive = true;
    giveSemaphore();
	auto self = shared_from_this();
	TaskFactory::createNonPriorityTask<TransientWorker>(
		TransientWorker::runTask,
		self,
		TAG
	);
	return true;
}
void TransientWorker::runTask(std::shared_ptr<TransientWorker> selfPtr) {
	selfPtr->taskLoop();
}

void TransientWorker::taskLoop() {
	Job* jobPtr = nullptr;
	while(true) {
		if (xQueueReceive(_queue, &jobPtr, _idleTicks) == pdTRUE) {
			std::unique_ptr<Job> job(jobPtr);
			(*job)();
			continue;
		}
		takeSemaphore();
		if (uxQueueMessagesWaiting(_queue) <= 0) {
			_alive = false;
			giveSemaphore();
			break;
		}
		giveSemaphore();
	}
	Log::Info(TAG, "Worker exiting after idle timeout");
	vTaskDelete(nullptr);
}
void TransientWorker::takeSemaphore(){
	if(xSemaphoreTake(_mutex, portMAX_DELAY)!=pdTRUE){
		Aborter::safeAbort(TAG, "Failed to take semaphore");
	}
}
void TransientWorker::giveSemaphore(){
	xSemaphoreGive(_mutex);
}
