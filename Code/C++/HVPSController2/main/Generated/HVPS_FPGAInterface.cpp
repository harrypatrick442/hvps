#include "Macros/GetFileName.hpp"
#include "HVPS_FPGAInterface.hpp"
#include "Timing/TimeHelper.hpp"
const char* HVPS_FPGAInterface::getTag() {return GET_FILE_NAME;}
HVPS_FPGAInterface::HVPS_FPGAInterface(IFPGABus& fpgaBus):_fpgaInterface(16,1089, fpgaBus, 150){}
uint64_t HVPS_FPGAInterface::getLastUpdateTimeUs(){ return _fpgaInterface.getLastUpdateTimeUs();}
void HVPS_FPGAInterface::setCommand(bool (&value)[8]){
     return _fpgaInterface.setBoolArray(0, value, 8);
}
void HVPS_FPGAInterface::setDesiredOutputVoltage(uint8_t value){
     return _fpgaInterface.setByte(8, value);
}
void HVPS_FPGAInterface::getBufferedData(uint8_t (&value)[128]){
    _fpgaInterface.getByteArray(16, value, 128);
}
uint8_t HVPS_FPGAInterface::getEchoDesiredOutputVoltage(){
     return _fpgaInterface.getByte(1040);
}
uint8_t HVPS_FPGAInterface::getMaxPrimaryCurrent(){
     return _fpgaInterface.getByte(1048);
}
uint8_t HVPS_FPGAInterface::getMaxOutputVoltage(){
     return _fpgaInterface.getByte(1056);
}
uint8_t HVPS_FPGAInterface::getMaxFirstStageVoltage(){
     return _fpgaInterface.getByte(1064);
}
void HVPS_FPGAInterface::getState(bool (&value)[8]){
    _fpgaInterface.getBoolArray(1072, value, 8);
}
uint8_t HVPS_FPGAInterface::getActualPeakPrimaryCurrent(){
     return _fpgaInterface.getByte(1080);
}
uint8_t HVPS_FPGAInterface::getActualOutputVoltage(){
     return _fpgaInterface.getByte(1088);
}
uint8_t HVPS_FPGAInterface::getActualFirstStageVoltage(){
     return _fpgaInterface.getByte(1096);
}
bool HVPS_FPGAInterface::getDoneCommand(){
     return _fpgaInterface.getBit(1104);
}
void HVPS_FPGAInterface::getAllFeedbacks(uint8_t& actualFirstStageVoltage, uint8_t& actualOutputVoltage, uint8_t& actualPeakPrimaryCurrent){
    _fpgaInterface.usingLocked([&](LockedFPGAInterface locked){
        actualFirstStageVoltage = locked.getByte(1096);
        actualOutputVoltage = locked.getByte(1088);
        actualPeakPrimaryCurrent = locked.getByte(1080);
    });
}
