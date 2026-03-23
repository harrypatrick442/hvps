#pragma once

#include <cstdint>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class FPGAInterface:public SingletonBase<FPGAInterface>
{
private:
    static constexpr uint64_t MINIMUM_UPDATE_PERIOD_MS = 50;

	IFPGABus& _fpgaBus;
    size_t _inputsLength;
    size_t _outputsLength;
	size_t _fullOutputBufferLength;

    bool* _inputBuffer;       // Inputs to shift into FPGA (ESP -> FPGA)
    bool* _fullOutputBuffer;  // Full readback buffer: [inputs_staging | outputs] (FPGA -> ESP)

    std::atomic<bool> _disposed;
	volatile bool _taskFinished;
    bool _inputsChanged;

    std::mutex _lock;

public:
    void setBit(size_t index, bool value);
    void setByte(size_t indexFrom, uint8_t value);
    void setUInt16(size_t indexFrom, uint16_t value);

    bool     getBit(size_t index);
    uint8_t  getByte(size_t indexFrom);
    uint16_t getUInt16(size_t indexFrom);

    void dispose();

    virtual ~FPGAInterface();

private:
    FPGAInterface(size_t inputsLength, size_t outputsLength, IFPGABus* fpgaBus);

    void shiftValuesIn();
    void readOutputs(bool includingStaging);
    bool validateStagedInputs();
    void setStagedInputsLive();
    void sleep();
    void startLooping();
    void loop();
    void doLoopSleep(int64_t& startTime);
};
