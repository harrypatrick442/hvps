#pragma once
#include "IFPGABus.hpp"
#include "Core/SingletonBase.hpp"
#include <cstdint>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class FPGAInterface:public SingletonBase<FPGAInterface>
{
private:
    static constexpr uint64_t MINIMUM_UPDATE_PERIOD_MS = 50;

    size_t _inputsLength;
    size_t _outputsLength;
	size_t _fullOutputBufferLength;

    bool* _inputBuffer;       // Inputs to shift into FPGA (ESP -> FPGA)
    bool* _fullOutputBuffer;  // Full readback buffer: [inputs_staging | outputs] (FPGA -> ESP)

	IFPGABus& _fpgaBus;
	
    std::atomic<bool> _disposed;
	volatile bool _taskFinished;
    bool _inputsChanged;
    std::atomic<uint64_t> _lastUpdateTimeUs;

    std::mutex _lockInputBuffer;
    std::mutex _lockFullOutputsBuffer;

public:
    FPGAInterface(size_t inputsLength, size_t outputsLength, IFPGABus& fpgaBus);
	uint64_t getLastUpdateTimeUs();
    void setBit(size_t index, bool value);
    void setByte(size_t indexFrom, uint8_t value);
    void setUInt16(size_t indexFrom, uint16_t value);

    bool     getBit(size_t index);
    uint8_t  getByte(size_t indexFrom);
    uint16_t getUInt16(size_t indexFrom);

    void dispose();

    virtual ~FPGAInterface();

private:
    void shiftValuesIn(bool* temporaryInputBuffer);
    void readOutputs(bool includingStaging, bool* temporaryFullOutputBuffer);
    bool validateStagedInputs(bool* temporaryInputBuffer, bool* temporaryFullOutputBuffer);
    void setStagedInputsLive();
    void sleep();
    void startLooping();
    void loop();
    void doLoopSleep(int64_t& startTime);
};
