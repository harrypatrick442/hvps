#include "HVPS_FPGAInterface.hpp"
#include "Timing/TimeHelper.hpp"
HVPS_FPGAInterface::HVPS_FPGAInterface(IFPGABus& fpgaBus):_fpgaInterface(26,25, fpgaBus){}
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
bool HVPS_FPGAInterface::getError(){
     return _fpgaInterface.getBit(50);
}
