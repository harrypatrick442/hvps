#ifndef TaskFactory_hpp
#define TaskFactory_hpp
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TaskFactory {
public:
    static constexpr int CORE_NON_PRIORITY = 0;
    static constexpr int CORE_PRIORITY = 1;
    static constexpr int STACK_SIZE_NON_PRIORITY = 8192;
    static constexpr int STACK_SIZE_PRIORITY = 8192;
    static constexpr int PRIORITY_NORMAL = 1;
    static constexpr int PRIORITY_HIGH = 5;
    static void createNonPriorityTask(void (*taskFunc)(void*), void* obj, const char* name,
		TaskHandle_t* _taskHandle = nullptr);
    static void createPriorityTask(void (*taskFunc)(void*), void* obj, const char* name,
		TaskHandle_t* _taskHandle = nullptr);
};

#endif // TaskFactory_hpp
