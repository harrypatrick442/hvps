#include "HardwareTimerPool.hpp"
#include "System/SafeAbort.hpp"

HardwareTimerInfo HardwareTimerPool::_timerInfos[N_INFOS] = {
    { TIMER_GROUP_1, TIMER_1 },
    { TIMER_GROUP_1, TIMER_0 },
    { TIMER_GROUP_0, TIMER_1 },
    { TIMER_GROUP_0, TIMER_0 }
};

std::mutex HardwareTimerPool::_mutex;

std::unique_ptr<HardwareTimerLease> HardwareTimerPool::acquire(std::string ownerName)
{
    std::unique_lock<std::mutex> lock(_mutex);

	size_t index = 0;
	do
	{
		HardwareTimerInfo& timerInfo = _timerInfos[index++];
		if(timerInfo.getInUse()){
			continue;
		}
		timerInfo.setUsing(ownerName);
		return std::make_unique<HardwareTimerLease>(timerInfo.getGroup(), timerInfo.getIndex(), [&, &timerInfo](){
			std::unique_lock<std::mutex> l(_mutex);
			timerInfo.setNotUsing();
		});
	}while(index<N_INFOS);
    SAFE_ABORT("No hardware timers available for %s", ownerName);
    __builtin_unreachable();
}
