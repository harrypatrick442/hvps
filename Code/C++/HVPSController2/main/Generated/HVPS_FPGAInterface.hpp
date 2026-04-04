#pragma once
#include "FPGA/FPGAInterface.hpp"
#include "FPGA/IFPGABus.hpp"
class HVPS_FPGAInterface {
private:
    FPGAInterface _fpgaInterface;
public:
    HVPS_FPGAInterface(IFPGABus& fpgaBus);
    uint64_t getLastUpdateTimeUs();
    void setDrive(bool value);
    void setDrive2(bool value);
    void setDesiredMaxFirstStageVoltage(uint8_t value);
    void setDesiredOutputVoltage(uint8_t value);
    void setDesiredMaxPeakPrimaryCurrent(uint8_t value);
    uint8_t getActualFirstStageVoltage();
    uint8_t getActualOutputVoltage();
    uint8_t getActualPeakPrimaryCurrent();
    uint8_t getActualFirstStageVoltage2();
    uint8_t getActualOutputVoltage2();
    uint8_t getActualPeakPrimaryCurrent2();
    bool getError();
    void getAllFeedbacks(uint8_t& actualFirstStageVoltage, uint8_t& actualOutputVoltage, uint8_t& actualPeakPrimaryCurrent, uint8_t& actualFirstStageVoltage2, uint8_t& actualOutputVoltage2, uint8_t& actualPeakPrimaryCurrent2);
};
