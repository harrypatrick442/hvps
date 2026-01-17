#include "Watchdog.hpp"
Watchdog::Watchdog(const char* name, uint32_t countFrom):
_countFrom(countFrom),
_count(count),
_name(name){

}
void Watchdog::run(){
	_count= _countFrom;
}
bool Watchdog::check(){
	if(_count==0){
		return false;
	}
	_count--;
	return true;
}
const char* Watchdog::getName(){
	return _name;
}