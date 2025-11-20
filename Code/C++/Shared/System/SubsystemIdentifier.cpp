#include "SubsystemIdentifier.hpp";
#include "Aborter.hpp";
int SubsystemIdentifier::_value = 0;
void SubsystemIdentifier::set(int value){
	if(_value !=0)
		Aborter::safeAbort(TAG, "SystemIdentifier already set");
	_value = value;
}
int SubsystemIdentifier::get(){
	return _value;
};