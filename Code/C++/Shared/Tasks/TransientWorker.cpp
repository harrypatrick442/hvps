#include "TransientWorker.hpp"
#include "System/SafeAbort.hpp"
#include "TaskFactory.hpp"
#include "../Timing/TimeHelper.hpp"
TransientWorker::TransientWorker(
	UBaseType_t maxQueueLength,
	uint32_t idleTimeoutMs,
	bool abortOnQueueOverflow,
	uint64_t maxDesiredJobTimeUs
)
    : 
	_idleTimeoutMs(idleTimeoutMs),
	_abortOnQueueOverflow(abortOnQueueOverflow),
	_maxDesiredJobTimeUs(maxDesiredJobTimeUs),
	_queue(xQueueCreate(maxQueueLength, sizeof(Job*))),
	_mutex(xSemaphoreCreateMutex()),
	_alive(false),
	_idleTicks(pdMS_TO_TICKS(idleTimeoutMs)){
		if (!_queue || !_mutex) {
			SAFE_ABORT("Failed to initialize TransientWorker resources");
		}
	}

TransientWorker::~TransientWorker() {
    if (_queue){
		taskLoop();
		vQueueDelete(_queue);
	}
    if (_mutex) vSemaphoreDelete(_mutex);
}

bool TransientWorker::enqueue(Job job) {
	Job* j = new (std::nothrow) Job(std::move(job));
    if (xQueueSend(_queue, &j, 0) != pdPASS) {
		if(_abortOnQueueOverflow){
			SAFE_ABORT("Queue overflowed");
			return false;
		}
        LOG_WARN("TransientWorker queue full; dropping job.");
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
	bool startedTaskSuccessfully = TaskFactory::createNonPriorityTask<TransientWorker>(
		TransientWorker::runTask,
		self,
		"TransientWorker"
	);
	if(!startedTaskSuccessfully){
		LOG_WARN("Failed to start runTask successfully");
		return false;
	}
	return true;
}
void TransientWorker::runTask(std::shared_ptr<TransientWorker> selfPtr) {
	selfPtr->taskLoop();
	//LOG_INFO("Worker exiting after idle timeout");
	vTaskDelete(nullptr);
}

void TransientWorker::taskLoop() {
	Job* jobPtr = nullptr;
	uint64_t startTimeUs = TimeHelper::us();
	while(true) {
		if (xQueueReceive(_queue, &jobPtr, _idleTicks) == pdTRUE) {
			std::unique_ptr<Job> job(jobPtr);
			(*job)();
			uint64_t endTimeUs = TimeHelper::us();
			if(endTimeUs - startTimeUs > _maxDesiredJobTimeUs){
				LOG_WARN("Warning, a task caused a delay greater than max allowed job time: %" PRIu64 " us", _maxDesiredJobTimeUs);
			}
			startTimeUs = endTimeUs;
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
}
void TransientWorker::takeSemaphore(){
	if(xSemaphoreTake(_mutex, portMAX_DELAY)!=pdTRUE){
		SAFE_ABORT("Failed to take semaphore");
	}
}
void TransientWorker::giveSemaphore(){
	xSemaphoreGive(_mutex);
}
