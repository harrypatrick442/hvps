#pragma once
#include "IFPGABus.hpp"
class FPGABus:public IFPGABus
{
private:
	int _inShiftPin;  
	int _inValuePin; 
	int _toOutputPin;
	int _outShiftPin;
	int _outValuePin;
	int _goLivePin;
public:
	FPGABus(
int inShiftPin,  
int inValuePin,  
int toOutputPin,
int outShiftPin,
int outValuePin,
int goLivePin);
    ~FPGABus() override = default;
    void setGoLive(bool value)override;
    void setInShift(bool value)override;
    void setOutShift(bool value)override;
    void setToOutput(bool value)override;
    void setInValue(bool value)override;
    bool getOutValue()override;
};