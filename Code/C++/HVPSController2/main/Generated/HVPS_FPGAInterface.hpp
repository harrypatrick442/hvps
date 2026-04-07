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
    void setCommand(bool (&value)[8]);
    uint8_t getActualFirstStageVoltage();
    uint8_t getActualOutputVoltage();
    uint8_t getActualPeakPrimaryCurrent();
    uint8_t getActualFirstStageVoltage2();
    uint8_t getActualOutputVoltage2();
    uint8_t getActualPeakPrimaryCurrent2();
    bool getError();
    uint8_t getEchoDesiredMaxFirstStageVoltage();
    uint8_t getEchoDesiredDesiredOutputVoltage();
    uint8_t getEchoDesiredMaxPeakPrimaryCurrent();
    void  getCollectedSample(uint8_t (&value)[120]);
    void getAllFeedbacks(uint8_t& actualFirstStageVoltage, uint8_t& actualOutputVoltage, uint8_t& actualPeakPrimaryCurrent, uint8_t& actualFirstStageVoltage2, uint8_t& actualOutputVoltage2, uint8_t& actualPeakPrimaryCurrent2);
};
