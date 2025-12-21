#include "PerformanceProfiler.hpp"
#include "../Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
//#include "esp_bt.h"
const char* PerformanceProfiler::TAG = "PerformanceProfiler";
IRAM_ATTR void PerformanceProfiler::profileMaxDelay(){
	uint64_t lastTime = esp_timer_get_time();
	uint64_t doneTime = lastTime + 1000000;
	uint64_t maxDelay = 0;
	int nLoops = 0;

	while(true){
		
		uint64_t time = esp_timer_get_time();
		uint64_t delay  = time - lastTime;
		lastTime = time;
		nLoops++;
		if(delay>maxDelay){
			maxDelay = delay;
		}
		if(time> doneTime){
			float average = 1000000.0f/nLoops;
			LOG_INFO("max delay was:  %" PRIu64, maxDelay);
			LOG_INFO("nLoops : %d", nLoops);
			LOG_INFO("average delay was : %f", average);
			LOG_INFO("core is : %d", xPortGetCoreID());
			maxDelay= 0;
			nLoops = 0;
			doneTime = time + 1000000;
			lastTime = esp_timer_get_time();
			
		}
		
		//LOG_INFO("VoltageFeedbackModule", "Sending");
		//Outputs::setThresholdReachedFiberOpticOnOff(on);
		//on = !on;
		
	/*	PingMessage pingMessage = PingMessage();
		fiberOpticChannel_1.sendMessage(pingMessage.toJSON());
		Delay::ms(1000);
		*/
		//int value = Inputs::readReceiver();
		//LOG_INFO("VoltageFeedbackModule", "Receiver: %d", value);
		//Delay::ms(1000);
	}
}