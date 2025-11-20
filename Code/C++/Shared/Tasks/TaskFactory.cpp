#include "TaskFactory.hpp"
void TaskFactory::createNonPriorityTask(
								  const std::function<void()>& fn,
								  const char* name,
								  TaskHandle_t* taskHandle)
{
    launchTrampolineTask(fn,
						name, 
                        STACK_SIZE_NON_PRIORITY,
                        PRIORITY_NORMAL,
                        CORE_NON_PRIORITY,
                        taskHandle);
}

void TaskFactory::createPriorityTask(
								  const std::function<void()>& fn,
								  const char* name,
								  TaskHandle_t* taskHandle)
{
    launchTrampolineTask(fn,
						name, 
                        STACK_SIZE_PRIORITY,
                        PRIORITY_HIGH,
                        CORE_PRIORITY,
                        taskHandle);
}
void TaskFactory::launchTrampolineTask(
                                 const std::function<void()>& fn,
								 const char* name,
                                 size_t stackSize,
                                 int priority,
                                 int core,
                                 TaskHandle_t* taskHandle) {
    auto* wrapper = new std::function<void()>(fn);

    auto trampoline = [](void* param) {
        std::unique_ptr<std::function<void()>> fnPtr(static_cast<std::function<void()>*>(param));
        (*fnPtr)();
    };

    BaseType_t result = xTaskCreatePinnedToCore(
        trampoline,
        name,
        stackSize,
        wrapper,
        priority,
        taskHandle,
        core
    );

    if (result != pdPASS) {
        delete wrapper;
        Aborter::safeAbort("TaskFactory", "Failed to create task: %s", name);
    }
}

// --- Raw pointer versions (original) ---
void TaskFactory::createNonPriorityTask(void (*taskFunc)(void*),
								  void* obj,
								  const char* name,
								  TaskHandle_t* taskHandle)
{
	BaseType_t result = xTaskCreatePinnedToCore(
		taskFunc,
		name,
		STACK_SIZE_NON_PRIORITY,
		obj,
		PRIORITY_NORMAL,
		taskHandle,
		CORE_NON_PRIORITY
	);

	if (result != pdPASS) {
		Aborter::safeAbort(TAG, "Failed to create non-priority task: %s", name);
	}
}

void TaskFactory::createPriorityTask(void (*taskFunc)(void*),
							   void* obj,
							   const char* name,
							   TaskHandle_t* taskHandle)
{
	BaseType_t result = xTaskCreatePinnedToCore(
		taskFunc,
		name,
		STACK_SIZE_PRIORITY,
		obj,
		PRIORITY_HIGH,
		taskHandle,
		CORE_PRIORITY
	);

	if (result != pdPASS) {
		Aborter::safeAbort(TAG, "Failed to create priority task: %s", name);
	}
}