#pragma once
#include <string>
#include "driver/timer.h"
class HardwareTimerInfo{
private:
    timer_group_t _group;
    timer_idx_t   _index;
	bool _inUse;
	std::string _ownerName;
public:
	HardwareTimerInfo(timer_group_t group, timer_idx_t index);
	timer_group_t getGroup();
	timer_idx_t getIndex();
	bool getInUse();
	void setUsing(std::string ownerName);
	void setNotUsing();
};