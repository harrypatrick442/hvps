#include "HVPS_FPGAInterface.hpp"
HVPS_FPGAInterface::HVPS_FPGAInterface(IFPGABus& fpgaBus):_fpgaInterface(18,24, fpgaBus){}
void HVPS_FPGAInterface::setDrive(bool value){
     return _fpgaInterface.setBit(0, value);
}
void HVPS_FPGAInterface::setDrive2(bool value){
     return _fpgaInterface.setBit(1, value);
}
void HVPS_FPGAInterface::setMaxFirstStageVoltageAllowed(uint8_t value){
     return _fpgaInterface.setByte(2, value);
}
void HVPS_FPGAInterface::setDesiredOutputVoltage(uint8_t value){
     return _fpgaInterface.setByte(10, value);
}
uint8_t HVPS_FPGAInterface::getActualFirstStageVoltage(){
     return _fpgaInterface.getByte(18);
}
uint8_t HVPS_FPGAInterface::getActualOutputVoltage(){
     return _fpgaInterface.getByte(26);
}
uint8_t HVPS_FPGAInterface::getActualPrimaryCurrent(){
     return _fpgaInterface.getByte(34);
}
