#pragma once
#include "FPGA/FPGAInterface.hpp"
#include "FPGA/IFPGABus.hpp"
#include "Core/SingletonBase.hpp"
class HVPS_FPGAInterface final : public SingletonBase<HVPS_FPGAInterface> {
public:
    static const char* getTag();
private:
    friend class SingletonBase<HVPS_FPGAInterface>;
    FPGAInterface _fpgaInterface;
public:
    HVPS_FPGAInterface(IFPGABus& fpgaBus);
    uint64_t getLastUpdateTimeUs();
    void setCommand(bool (&value)[8]);
    void setDesiredOutputVoltage(uint8_t value);
    void setDrive2(bool value);
    void setDrive(bool value);
    void  getBufferedData(uint8_t (&value)[128]);
    uint8_t getEchoDesiredOutputVoltage();
    uint8_t getMaxPrimaryCurrent();
    uint8_t getMaxOutputVoltage();
    uint8_t getMaxFirstStageVoltage();
    bool getError();
    uint8_t getActualPeakPrimaryCurrent();
    uint8_t getActualOutputVoltage();
    uint8_t getActualFirstStageVoltage();
    void getAllFeedbacks(uint8_t& actualFirstStageVoltage, uint8_t& actualOutputVoltage, uint8_t& actualPeakPrimaryCurrent);
};
