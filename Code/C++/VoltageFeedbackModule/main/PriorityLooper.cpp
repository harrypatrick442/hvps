#include "PriorityLooper.hpp"
#include "Tasks/TaskFactory.hpp"
#include "System/WatchdogFeeder.hpp"
#include "Logging/Log.hpp"
#include "System/PerformanceProfiler.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
const char* PriorityLooper::TAG = "PriorityLooper";
void PriorityLooper::start(){
	TaskFactory::createPriorityTask(
		run,
		NULL,
		"PriorityLooper"
    );
}
void PriorityLooper::run(void*){
	//PerformanceProfiler::profileMaxDelay();
}