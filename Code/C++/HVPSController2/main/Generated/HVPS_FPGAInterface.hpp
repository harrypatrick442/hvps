#pragma once
#include "FPGA/FPGAInterface.hpp"
#include "FPGA/IFPGABus.hpp"
class HVPS_FPGAInterface {
private:
    FPGAInterface _fpgaInterface;
public:
    HVPS_FPGAInterface(IFPGABus& fpgaBus);
    void setDrive(bool value);
    void setDrive2(bool value);
    void setMaxFirstStageVoltageAllowed(uint8_t value);
    void setDesiredOutputVoltage(uint8_t value);
    uint8_t getActualFirstStageVoltage();
    uint8_t getActualOutputVoltage();
    uint8_t getActualPrimaryCurrent();
};
