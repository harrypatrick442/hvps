#include "FPGABUS.hpp"
#include "IO/IOHelper.hpp"
FPGABus::FPGABus(
int inShiftPin,  
int inValuePin,  
int toOutputPin,
int outShiftPin,
int outValuePin,
int goLivePin):IFPGABus(),
_inShiftPin(inShiftPin),  
_inValuePin(inValuePin),  
_toOutputPin(toOutputPin),
_outShiftPin(outShiftPin),
_outValuePin(outValuePin),
_goLivePin(goLivePin){
	IOHelper::configureOutputPin(inShiftPin, false);
	IOHelper::configureOutputPin(inValuePin, false);
	IOHelper::configureOutputPin(toOutputPin, false);
	IOHelper::configureOutputPin(outShiftPin, false);
	IOHelper::configureInputPin(outValuePin, std::nullopt);
	IOHelper::configureOutputPin(goLivePin, false);
}
void FPGABus::setGoLive(bool value){
	IOHelper::setOnElseOff(_goLivePin, value);
}
void FPGABus::setInShift(bool value){
	IOHelper::setOnElseOff(_inShiftPin, value);
}
void FPGABus::setOutShift(bool value){
	IOHelper::setOnElseOff(_outShiftPin, value);
}
void FPGABus::setToOutput(bool value){
	IOHelper::setOnElseOff(_toOutputPin, value);
}
void FPGABus::setInValue(bool value){
	IOHelper::setOnElseOff(_inValuePin, value);
}
bool FPGABus::getOutValue(){
	return IOHelper::getIsHigh(_outValuePin);
}