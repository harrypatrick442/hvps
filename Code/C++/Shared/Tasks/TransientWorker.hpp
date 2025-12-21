#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <functional>
#include <memory>
#include <atomic>
#include "TaskFactory.hpp"
#include "../Logging/Log.hpp"

class TransientWorker : public std::enable_shared_from_this<TransientWorker> 
/* Keep self alive during task execution*/
{
public:
    using Job = std::function<void()>;
/**
 * @brief Constructs a new TransientWorker. Also known as a sequential scheduler by me.
 *
 * @param maxQueueLength Maximum number of jobs that can be enqueued.
 * @param idleTimeoutMs Time in milliseconds before the worker auto-exits when idle.
 * @param abortOnQueueOverflow Whether to abort if the queue is full.
 */
    explicit TransientWorker(
		UBaseType_t maxQueueLength=32, 
		uint32_t idleTimeoutMs=500, 
		bool abortOnQueueOverflow = true,
		uint64_t maxDesiredJobTimeUs = 10000);
    ~TransientWorker();

    bool enqueue(Job job);

private:
    const uint32_t _idleTimeoutMs;
	const bool _abortOnQueueOverflow;
	uint64_t _maxDesiredJobTimeUs;
    QueueHandle_t _queue;
    SemaphoreHandle_t _mutex;
    std::atomic<bool> _alive;
	TickType_t _idleTicks;
    void taskLoop();
	static void runTask(std::shared_ptr<TransientWorker> selfPtr);
	void takeSemaphore();
	void giveSemaphore();
};
