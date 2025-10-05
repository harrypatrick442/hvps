#pragma once

#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../System/Aborter.hpp"
#include "../Logging/Log.hpp"

class TaskFactory {
private:
    inline static constexpr const char* TAG = "TaskFactory";
    inline static constexpr int CORE_NON_PRIORITY = 0;
    inline static constexpr int CORE_PRIORITY = 1;
    inline static constexpr size_t  STACK_SIZE_NON_PRIORITY = 8192;
    inline static constexpr size_t  STACK_SIZE_PRIORITY = 8192;
    inline static constexpr int PRIORITY_NORMAL = 1;
    inline static constexpr int PRIORITY_HIGH = 5;
public:
    // --- Raw pointer versions (original) ---
    static void createNonPriorityTask(void (*taskFunc)(void*),
                                      void* obj,
                                      const char* name,
                                      TaskHandle_t* taskHandle = nullptr)
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

    static void createPriorityTask(void (*taskFunc)(void*),
                                   void* obj,
                                   const char* name,
                                   TaskHandle_t* taskHandle = nullptr)
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

    // --- Shared pointer versions (new) ---// --- Shared pointer version (non-priority) ---
	template <typename T>
	static void createNonPriorityTask(void (*taskFunc)(std::shared_ptr<T>),
									  std::shared_ptr<T> obj,
									  const char* name,
									  TaskHandle_t* taskHandle = nullptr)
	{
		struct Wrapper {
			void (*func)(std::shared_ptr<T>);
			std::shared_ptr<T> obj;
		};

		auto* wrapper = new Wrapper{ taskFunc, std::move(obj) };

		auto trampoline = [](void* param) {
			// take ownership of wrapper so it is deleted when task ends
			std::unique_ptr<Wrapper> w(static_cast<Wrapper*>(param));
			w->func(w->obj);
		};

		BaseType_t result = xTaskCreatePinnedToCore(
			trampoline,             // <-- plain function pointer OK
			name,
			STACK_SIZE_NON_PRIORITY,
			wrapper,
			PRIORITY_NORMAL,
			taskHandle,
			CORE_NON_PRIORITY
		);

		if (result != pdPASS) {
			delete wrapper;
			Aborter::safeAbort(TAG, "Failed to create non-priority task: %s", name);
		}
	}
	template <typename T>
	static void createPriorityTask(void (*taskFunc)(std::shared_ptr<T>),
								   std::shared_ptr<T> obj,
								   const char* name,
								   TaskHandle_t* _taskHandle = nullptr)
	{
		struct Wrapper {
			void (*func)(std::shared_ptr<T>);
			std::shared_ptr<T> obj;
		};

		auto* wrapper = new Wrapper{ taskFunc, std::move(obj) };

		auto trampoline = [](void* param) {
			// take ownership of wrapper so it's deleted when done
			std::unique_ptr<Wrapper> w(static_cast<Wrapper*>(param));
			w->func(w->obj);
		};

		BaseType_t result = xTaskCreatePinnedToCore(
			trampoline,     // <-- this is now a plain non-capturing lambda
			name,
			STACK_SIZE_PRIORITY,
			wrapper,
			PRIORITY_HIGH,
			_taskHandle,
			CORE_PRIORITY
		);

		if (result != pdPASS) {
			delete wrapper; // cleanup if task creation failed
			Aborter::safeAbort(TAG, "Failed to create priority task: %s", name);
		}
	}

};
