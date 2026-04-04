#include "HVPS_FPGAInterface.hpp"
#include "Timing/TimeHelper.hpp"
HVPS_FPGAInterface::HVPS_FPGAInterface(IFPGABus& fpgaBus):_fpgaInterface(26,49, fpgaBus, 10){}
uint64_t HVPS_FPGAInterface::getLastUpdateTimeUs(){ return _fpgaInterface.getLastUpdateTimeUs();}
void HVPS_FPGAInterface::setDrive(bool value){
     return _fpgaInterface.setBit(0, value);
}
void HVPS_FPGAInterface::setDrive2(bool value){
     return _fpgaInterface.setBit(1, value);
}
void HVPS_FPGAInterface::setDesiredMaxFirstStageVoltage(uint8_t value){
     return _fpgaInterface.setByte(2, value);
}
void HVPS_FPGAInterface::setDesiredOutputVoltage(uint8_t value){
     return _fpgaInterface.setByte(10, value);
}
void HVPS_FPGAInterface::setDesiredMaxPeakPrimaryCurrent(uint8_t value){
     return _fpgaInterface.setByte(18, value);
}
uint8_t HVPS_FPGAInterface::getActualFirstStageVoltage(){
     return _fpgaInterface.getByte(26);
}
uint8_t HVPS_FPGAInterface::getActualOutputVoltage(){
     return _fpgaInterface.getByte(34);
}
uint8_t HVPS_FPGAInterface::getActualPeakPrimaryCurrent(){
     return _fpgaInterface.getByte(42);
}
uint8_t HVPS_FPGAInterface::getActualFirstStageVoltage2(){
     return _fpgaInterface.getByte(50);
}
uint8_t HVPS_FPGAInterface::getActualOutputVoltage2(){
     return _fpgaInterface.getByte(58);
}
uint8_t HVPS_FPGAInterface::getActualPeakPrimaryCurrent2(){
     return _fpgaInterface.getByte(66);
}
bool HVPS_FPGAInterface::getError(){
     return _fpgaInterface.getBit(74);
}
void HVPS_FPGAInterface::getAllFeedbacks(uint8_t& actualFirstStageVoltage, uint8_t& actualOutputVoltage, uint8_t& actualPeakPrimaryCurrent, uint8_t& actualFirstStageVoltage2, uint8_t& actualOutputVoltage2, uint8_t& actualPeakPrimaryCurrent2){
    _fpgaInterface.usingLocked([&](LockedFPGAInterface locked){
        actualFirstStageVoltage = locked.getByte(26);
        actualOutputVoltage = locked.getByte(34);
        actualPeakPrimaryCurrent = locked.getByte(42);
        actualFirstStageVoltage2 = locked.getByte(50);
        actualOutputVoltage2 = locked.getByte(58);
        actualPeakPrimaryCurrent2 = locked.getByte(66);
    });
}
