#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <functional>
#include <memory>
#include <atomic>
#include "TaskFactory.hpp"
#include "../Logging/Log.hpp"

class TransientWorker : public std::enable_shared_from_this<TransientWorker> {
public:
    using Job = std::function<void()>;

    explicit TransientWorker(uint32_t idleTimeoutMs = 2000);
    ~TransientWorker();

    void enqueue(Job job);

private:
    static constexpr const char* TAG = "TransientWorker";
    static constexpr size_t kQueueDepth = 16;

    QueueHandle_t _queue;
    SemaphoreHandle_t _mutex;
    std::atomic<bool> _alive;
    const uint32_t _idleTimeoutMs;

    void taskLoop();
};
