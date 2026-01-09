#pragma once

#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "System/SafeAbort.hpp"
#include "System/MemoryHelper.hpp"
#include "../Logging/Log.hpp"

class TaskFactory {
private:
    inline static constexpr int CORE_NON_PRIORITY = 0;
    inline static constexpr int CORE_PRIORITY = 1;
    inline static constexpr size_t  STACK_SIZE_NON_PRIORITY = 8192;
    inline static constexpr size_t  STACK_SIZE_PRIORITY = 8192;
    inline static constexpr int PRIORITY_NORMAL = 1;
    inline static constexpr int PRIORITY_HIGH = 5;
public:
	static bool createNonPriorityTask(
									  const std::function<void()>& fn,
									  const char* name,
									  TaskHandle_t* taskHandle = nullptr);
	static bool createPriorityTask(
									  const std::function<void()>& fn,
									  const char* name,
									  TaskHandle_t* taskHandle = nullptr);
    // --- Raw pointer versions (original) ---
    /*static bool createNonPriorityTask(void (*taskFunc)(void*),
                                      void* obj,
                                      const char* name,
                                      TaskHandle_t* taskHandle = nullptr);

    static bool createPriorityTask(void (*taskFunc)(void*),
                                   void* obj,
                                   const char* name,
                                   TaskHandle_t* taskHandle = nullptr);*/

    // --- Shared pointer versions (new) ---// --- Shared pointer version (non-priority) ---
	template <typename T>
	inline static bool createNonPriorityTask(void (*taskFunc)(std::shared_ptr<T>),
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
			{
				std::unique_ptr<Wrapper> w(static_cast<Wrapper*>(param));
				w->func(w->obj);
			}
			vTaskDelete(nullptr);
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
			SAFE_ABORT("Failed to create non-priority task: %s", name);
			return false;
		}
		return true;
	}
	template <typename T>
	inline static bool createPriorityTask(void (*taskFunc)(std::shared_ptr<T>),
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
			{
				std::unique_ptr<Wrapper> w(static_cast<Wrapper*>(param));
				w->func(w->obj);
			}
			vTaskDelete(nullptr);
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
			SAFE_ABORT("Failed to create priority task: %s", name);
			return false;
		}
		return true;
	}
private:
	static bool launchTrampolineTask(
                                 const std::function<void()>& fn,
								 const char* name,
                                 size_t stackSize,
                                 int priority,
                                 int core,
                                 TaskHandle_t* taskHandle);
};
