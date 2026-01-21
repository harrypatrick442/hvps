#include "HardwareTimerInfo.hpp"
HardwareTimerInfo::HardwareTimerInfo(timer_group_t group, timer_idx_t index):
	_group(group),
	_index(index),
	_inUse(false),
	_ownerName("none"){
	
}
timer_group_t HardwareTimerInfo::getGroup(){
	return _group;
}
timer_idx_t HardwareTimerInfo::getIndex(){
	return _index;
}
bool HardwareTimerInfo::getInUse(){
	return _inUse;
}
void HardwareTimerInfo::setUsing(std::string ownerName){
	_inUse = true;
	_ownerName = ownerName;
}
void HardwareTimerInfo::setNotUsing(){
	_inUse = false;
}