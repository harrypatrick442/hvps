#include "FPGAInterface.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Tasks/TaskFactory.hpp"
#include <chrono>
#include <cstring>

// ─── Constructor / Destructor ─────────────────────────────────────────────────

FPGAInterface::FPGAInterface(
    size_t inputsLength, size_t outputsLength, IFPGABus& fpgaBus)
    : _inputsLength(inputsLength)
    , _outputsLength(outputsLength)
	, _fullOutputBufferLength(inputsLength + outputsLength)
	, _inputBuffer(nullptr)
	, _fullOutputBuffer(nullptr)
    , _fpgaBus(fpgaBus)
    , _disposed(false)
	, _taskFinished(false)
    , _inputsChanged(true)
	, _lastUpdateTimeUs(0)
{
    _inputBuffer      = new bool[inputsLength]();
    _fullOutputBuffer = new bool[_fullOutputBufferLength]();

    _fpgaBus.setGoLive(false);
    _fpgaBus.setInShift(false);
    _fpgaBus.setOutShift(false);
    _fpgaBus.setToOutput(false);

    startLooping();
}

FPGAInterface::~FPGAInterface()
{
    dispose();
	while (!_taskFinished) {
		Delay::ms(10);
	}
    delete[] _inputBuffer;
    delete[] _fullOutputBuffer;
}


uint64_t FPGAInterface::getLastUpdateTimeUs(){
	return _lastUpdateTimeUs.load(std::memory_order_relaxed);
}
// ─── Public: Input setters ────────────────────────────────────────────────────

void FPGAInterface::setBit(size_t index, bool value)
{
    std::lock_guard<std::mutex> guard(_lock);
    _inputBuffer[index] = value;
    _inputsChanged = true;
}

void FPGAInterface::setByte(size_t indexFrom, uint8_t value)
{
    std::lock_guard<std::mutex> guard(_lock);
    _inputBuffer[indexFrom + 0] = (value & 0x01) != 0;
    _inputBuffer[indexFrom + 1] = (value & 0x02) != 0;
    _inputBuffer[indexFrom + 2] = (value & 0x04) != 0;
    _inputBuffer[indexFrom + 3] = (value & 0x08) != 0;
    _inputBuffer[indexFrom + 4] = (value & 0x10) != 0;
    _inputBuffer[indexFrom + 5] = (value & 0x20) != 0;
    _inputBuffer[indexFrom + 6] = (value & 0x40) != 0;
    _inputBuffer[indexFrom + 7] = (value & 0x80) != 0;
    _inputsChanged = true;
}

void FPGAInterface::setUInt16(size_t indexFrom, uint16_t value)
{
    std::lock_guard<std::mutex> guard(_lock);
    bool* buf = _inputBuffer;
    buf[indexFrom + 0]  = (value & 0x0001) != 0;
    buf[indexFrom + 1]  = (value & 0x0002) != 0;
    buf[indexFrom + 2]  = (value & 0x0004) != 0;
    buf[indexFrom + 3]  = (value & 0x0008) != 0;
    buf[indexFrom + 4]  = (value & 0x0010) != 0;
    buf[indexFrom + 5]  = (value & 0x0020) != 0;
    buf[indexFrom + 6]  = (value & 0x0040) != 0;
    buf[indexFrom + 7]  = (value & 0x0080) != 0;
    buf[indexFrom + 8]  = (value & 0x0100) != 0;
    buf[indexFrom + 9]  = (value & 0x0200) != 0;
    buf[indexFrom + 10] = (value & 0x0400) != 0;
    buf[indexFrom + 11] = (value & 0x0800) != 0;
    buf[indexFrom + 12] = (value & 0x1000) != 0;
    buf[indexFrom + 13] = (value & 0x2000) != 0;
    buf[indexFrom + 14] = (value & 0x4000) != 0;
    buf[indexFrom + 15] = (value & 0x8000) != 0;
    _inputsChanged = true;
}

bool FPGAInterface::getBit(size_t index)
{
    std::lock_guard<std::mutex> guard(_lock);
    return _fullOutputBuffer[index];
}

uint8_t FPGAInterface::getByte(size_t indexFrom)
{
    std::lock_guard<std::mutex> guard(_lock);
    const bool* buf = _fullOutputBuffer;
    int result = 0;
    if (buf[indexFrom + 0]) result |= 0x01;
    if (buf[indexFrom + 1]) result |= 0x02;
    if (buf[indexFrom + 2]) result |= 0x04;
    if (buf[indexFrom + 3]) result |= 0x08;
    if (buf[indexFrom + 4]) result |= 0x10;
    if (buf[indexFrom + 5]) result |= 0x20;
    if (buf[indexFrom + 6]) result |= 0x40;
    if (buf[indexFrom + 7]) result |= 0x80;
    return static_cast<uint8_t>(result);
}

uint16_t FPGAInterface::getUInt16(size_t indexFrom)
{
    std::lock_guard<std::mutex> guard(_lock);
    const bool* buf = _fullOutputBuffer;
    uint16_t result = 0;
    if (buf[indexFrom + 0])  result |= 0x0001;
    if (buf[indexFrom + 1])  result |= 0x0002;
    if (buf[indexFrom + 2])  result |= 0x0004;
    if (buf[indexFrom + 3])  result |= 0x0008;
    if (buf[indexFrom + 4])  result |= 0x0010;
    if (buf[indexFrom + 5])  result |= 0x0020;
    if (buf[indexFrom + 6])  result |= 0x0040;
    if (buf[indexFrom + 7])  result |= 0x0080;
    if (buf[indexFrom + 8])  result |= 0x0100;
    if (buf[indexFrom + 9])  result |= 0x0200;
    if (buf[indexFrom + 10]) result |= 0x0400;
    if (buf[indexFrom + 11]) result |= 0x0800;
    if (buf[indexFrom + 12]) result |= 0x1000;
    if (buf[indexFrom + 13]) result |= 0x2000;
    if (buf[indexFrom + 14]) result |= 0x4000;
    if (buf[indexFrom + 15]) result |= 0x8000;
    return result;
}

// ─── Public: Lifecycle ────────────────────────────────────────────────────────

void FPGAInterface::dispose()
{
    _disposed.store(true);
}

// ─── Private: Bus operations ──────────────────────────────────────────────────

void FPGAInterface::setStagedInputsLive()
{
    _fpgaBus.setGoLive(true);
    sleep();
    _fpgaBus.setGoLive(false);
    sleep();
}

bool FPGAInterface::validateStagedInputs()
{
    for (size_t i = 0; i < _inputsLength; ++i)
    {
        if (_inputBuffer[i] != _fullOutputBuffer[i])
            return false;
    }
    return true;
}

void FPGAInterface::shiftValuesIn()
{
    // Shifts MSB first into FPGA
    size_t i = _inputsLength;
    while (i > 0)
    {
		i--;
        _fpgaBus.setInShift(false);
        _fpgaBus.setInValue(_inputBuffer[i]);
        sleep();
        _fpgaBus.setInShift(true);
        sleep();
    }
    _fpgaBus.setInShift(false);
}

void FPGAInterface::readOutputs(bool includingStaging)
{
    _fpgaBus.setToOutput(true);
    sleep();
    _fpgaBus.setToOutput(false);
    sleep();

    size_t length = includingStaging
        ? _fullOutputBufferLength
        : _outputsLength;

    size_t intoIndex = _fullOutputBufferLength - 1;

    for (size_t i = 0; i < length; ++i)
    {
        _fpgaBus.setOutShift(true);
        sleep();
        _fullOutputBuffer[intoIndex--] = _fpgaBus.getOutValue();
        _fpgaBus.setOutShift(false);
        sleep();
    }
}

void FPGAInterface::sleep()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

// ─── Private: Loop ────────────────────────────────────────────────────────────

void FPGAInterface::startLooping()
{
	if(!TaskFactory::createPriorityTask([this](){
			loop();
		}, "FPGAInterface::loop")
	)
	{
		SAFE_ABORT("Failed to start task");
	}
}

void FPGAInterface::loop()
{
    int64_t startTime = TimeHelper::ms();

    while (true)
    {
        {
            std::lock_guard<std::mutex> guard(_lock);

            if (_disposed.load()) {
				_taskFinished = true;
				return;
			}
			uint64_t timeUs = TimeHelper::us();
            if (!_inputsChanged)
            {
                readOutputs(false);
            }
            else
            {
                shiftValuesIn();
                readOutputs(true);
                if (validateStagedInputs())
                {
                    setStagedInputsLive();
                    _inputsChanged = false;
                }
            }
			_lastUpdateTimeUs.store(timeUs, std::memory_order_relaxed);
        }
        doLoopSleep(startTime);
    }
}

void FPGAInterface::doLoopSleep(int64_t& startTime)
{
    int64_t now = TimeHelper::ms();
    int64_t elapsed = now - startTime;
    startTime = now;

    int64_t toSleep = MINIMUM_UPDATE_PERIOD_MS - static_cast<int64_t>(elapsed);
    if (toSleep > 0)
        Delay::ms(toSleep);
}
