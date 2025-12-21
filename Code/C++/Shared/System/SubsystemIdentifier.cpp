#include "SubsystemIdentifier.hpp"
#include "SafeAbort.hpp"
int SubsystemIdentifier::_value = 0;
void SubsystemIdentifier::set(int value){
	if(_value !=0)
		SAFE_ABORT("SystemIdentifier already set");
	_value = value;
}
int SubsystemIdentifier::get(){
	return _value;
};