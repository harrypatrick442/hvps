#include "TaskFactory.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include <cstdlib>

static const char* TAG = "TaskFactory";

void TaskFactory::createNonPriorityTask(void (*taskFunc)(void*), void* obj, const char* name, 
TaskHandle_t* _taskHandle) {
    BaseType_t result = xTaskCreatePinnedToCore(
        taskFunc,                // Task function
        name,                    // Name for debugging
        STACK_SIZE_NON_PRIORITY,       // Stack size in words (not bytes)
        obj,                 // Task parameter
        PRIORITY_NORMAL,    // Priority
        _taskHandle,                 // Task handle (not used)
        CORE_NON_PRIORITY           // Pinned core
    );

    if (result != pdPASS) {
        Aborter::safeAbort(TAG, "Failed to create non-priority task: %s", name);
    }
}

void TaskFactory::createPriorityTask(void (*taskFunc)(void*), void* obj, const char* name, 
TaskHandle_t* _taskHandle) {
    BaseType_t result = xTaskCreatePinnedToCore(
        taskFunc,                // Task function
        name,                    // Name for debugging
        STACK_SIZE_PRIORITY,     // Stack size in words (not bytes)
        obj,                 // Task parameter
        PRIORITY_HIGH,  // Priority
        _taskHandle,                 // Task handle (not used)
        CORE_PRIORITY           // Pinned core
    );

    if (result != pdPASS) {
        Aborter::safeAbort(TAG, "Failed to create priority task: %s", name);
    }
}
