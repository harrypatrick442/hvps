#include "DebugHelper.hpp"
void DebugHelper::periodicallyPrint(){
	TaskFactory::createNonPriorityTask([this](){
			while(true){
				Delay::ms(200);
			}
			
		}, 
		"debugHelper"
	);
}