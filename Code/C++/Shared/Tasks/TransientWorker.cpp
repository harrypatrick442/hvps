#include "TransientWorker.hpp"

TransientWorker::TransientWorker(uint32_t idleTimeoutMs)
    : _queue(xQueueCreate(kQueueDepth, sizeof(Job*))),
      _mutex(xSemaphoreCreateMutex()),
      _alive(false),
      _idleTimeoutMs(idleTimeoutMs) {}

TransientWorker::~TransientWorker() {
    if (_queue) vQueueDelete(_queue);
    if (_mutex) vSemaphoreDelete(_mutex);
}

void TransientWorker::enqueue(Job job) {
    Job* j = new Job(std::move(job));

    if (xQueueSend(_queue, &j, 0) != pdPASS) {
        Log::Warn(TAG, "TransientWorker queue full; dropping job.");
        delete j;
        return;
    }

    xSemaphoreTake(_mutex, portMAX_DELAY);
    if (!_alive) {
        _alive = true;
        auto self = shared_from_this();
        TaskFactory::createNonPriorityTask<std::shared_ptr<TransientWorker>>(
            [](std::shared_ptr<TransientWorker> selfPtr) {
                selfPtr->taskLoop();
            },
            self,
            "TransientWorker"
        );
    }
    xSemaphoreGive(_mutex);
}

void TransientWorker::taskLoop() {
    Log::Info(TAG, "Worker started on low-speed core");
    Job* jobPtr = nullptr;
    TickType_t idleTicks = pdMS_TO_TICKS(_idleTimeoutMs);

    for (;;) {
        if (xQueueReceive(_queue, &jobPtr, idleTicks) == pdTRUE) {
            std::unique_ptr<Job> job(jobPtr);
            try {
                (*job)();
            } catch (...) {
                Log::Error(TAG, "Exception in transient job");
            }
        } else {
            break; // idle timeout
        }
    }

    xSemaphoreTake(_mutex, portMAX_DELAY);
    _alive = false;
    xSemaphoreGive(_mutex);

    Log::Info(TAG, "Worker exiting after idle timeout");
    vTaskDelete(nullptr);
}
