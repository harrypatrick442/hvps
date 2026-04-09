#include "FPGAInterface.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Logging/Log.hpp"
#include <chrono>
#include <cstring>

// ─── Constructor / Destructor ─────────────────────────────────────────────────

FPGAInterface::FPGAInterface(
    size_t inputsLength, size_t outputsLength, IFPGABus& fpgaBus, uint64_t sleepUs)
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
	, _sleepUs(sleepUs)
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
    std::lock_guard<std::mutex> guard(_lockInputBuffer);
    _inputBuffer[index] = value;
    _inputsChanged = true;
}

void FPGAInterface::setByte(size_t indexFrom, uint8_t value)
{
    std::lock_guard<std::mutex> guard(_lockInputBuffer);
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
    std::lock_guard<std::mutex> guard(_lockInputBuffer);
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
void FPGAInterface::setByteArray(size_t indexFrom, uint8_t* value, size_t length){
    std::lock_guard<std::mutex> guard(_lockInputBuffer);
    bool* buf = _inputBuffer;
	size_t i = 0;
	while(i<length){
		uint8_t byte  = value[i++];
		buf[indexFrom++]  = (byte & 0x0001) != 0;
		buf[indexFrom++]  = (byte & 0x0002) != 0;
		buf[indexFrom++]  = (byte & 0x0004) != 0;
		buf[indexFrom++]  = (byte & 0x0008) != 0;
		buf[indexFrom++]  = (byte & 0x0010) != 0;
		buf[indexFrom++]  = (byte & 0x0020) != 0;
		buf[indexFrom++]  = (byte & 0x0040) != 0;
		buf[indexFrom++]  = (byte & 0x0080) != 0;
	}
    _inputsChanged = true;
}
void FPGAInterface::setBoolArray(size_t indexFrom, bool* value, size_t length){
    std::lock_guard<std::mutex> guard(_lockInputBuffer);
    bool* buf = _inputBuffer;
	size_t i = 0;
	while(i<length){
		
		buf[indexFrom++]  = value[i++];
	}
    _inputsChanged = true;
}
void FPGAInterface::usingLocked(std::function<void(LockedFPGAInterface)> callback)
{
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
    LockedFPGAInterface locked(
        [this](size_t i)     { return getBitNoLock(i);     },
        [this](size_t i)     { return getByteNoLock(i);    },
        [this](size_t i)     { return getUInt16NoLock(i);  }
    );
    callback(locked);
}
bool FPGAInterface::getBit(size_t index)
{
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
    return getBitNoLock(index);
}
bool FPGAInterface::getBitNoLock(size_t index)
{
    return _fullOutputBuffer[index];
}

uint8_t FPGAInterface::getByte(size_t indexFrom)
{
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
	return getByteNoLock(indexFrom);
}
uint8_t FPGAInterface::getByteNoLock(size_t indexFrom)
{
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
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
    return getUInt16NoLock(indexFrom);
}
uint16_t FPGAInterface::getUInt16NoLock(size_t indexFrom)
{
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
void FPGAInterface::getByteArray(size_t indexFrom, uint8_t* bytes, size_t length)
{
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
    const bool* buf = _fullOutputBuffer;
	int i=0;
	while(i<length){
		uint8_t byte = 0;
		if (buf[indexFrom++])  byte |= 0x0001;
		if (buf[indexFrom++])  byte |= 0x0002;
		if (buf[indexFrom++])  byte |= 0x0004;
		if (buf[indexFrom++])  byte |= 0x0008;
		if (buf[indexFrom++])  byte |= 0x0010;
		if (buf[indexFrom++])  byte |= 0x0020;
		if (buf[indexFrom++])  byte |= 0x0040;
		if (buf[indexFrom++])  byte |= 0x0080;
		bytes[i++]=byte;
	}
}

void FPGAInterface::getBoolArray(size_t indexFrom, bool* bits, size_t length)
{
    std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
    const bool* buf = _fullOutputBuffer;
	int i=0;
	while(i<length){
		bits[i++]=buf[indexFrom++];
	}
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

bool FPGAInterface::validateStagedInputs(bool* temporaryInputBuffer, bool* temporaryFullOutputBuffer)
{
    for (size_t i = 0; i < _inputsLength; ++i)
    {
        if (temporaryInputBuffer[i] != temporaryFullOutputBuffer[i])
            return false;
    }
    return true;
}

void FPGAInterface::shiftValuesIn(bool* temporaryInputBuffer)
{
    // Shifts MSB first into FPGA
    size_t i = _inputsLength;
    while (i > 0)
    {
		i--;
        _fpgaBus.setInShift(false);
        _fpgaBus.setInValue(temporaryInputBuffer[i]);
        sleep();
        _fpgaBus.setInShift(true);
        sleep();
    }
    _fpgaBus.setInShift(false);
}

void FPGAInterface::readOutputs(bool includingStaging, bool* temporaryFullOutputBuffer)
{
    _fpgaBus.setToOutput(true);
    sleep();
    _fpgaBus.setToOutput(false);
    sleep();

    size_t length = includingStaging
        ? _fullOutputBufferLength
        : _outputsLength;

    size_t intoIndex = _fullOutputBufferLength - 1;

	//std::string binaryString(length, 'z');
    for (size_t i = 0; i < length; ++i)
    {
		bool value = _fpgaBus.getOutValue();
        sleep();
        temporaryFullOutputBuffer[intoIndex--] = value;
		/*
		if (value)
			binaryString[i] = '1';
		else{
			binaryString[i] = '0';
		}
		*/
        _fpgaBus.setOutShift(true);
        sleep();
        _fpgaBus.setOutShift(false);
        sleep();
    }
	/*
	in verilog 
	bit[7] = 1 (MSB)
	bit[0] = 1 (LSB)
	we get the msb out first. 
	we get the lsb out last.
	we work backwards on index. so now the msb is at the highest index. 
	
	*/
	//LOG_INFO(includingStaging?"Included staging":"Did not include staging");
	//LOG_INFO("what came out in order it came out from left ot right: %s", binaryString.c_str());
	/*size_t j = 34;
	size_t l = 0;
	while(l<4){		
		if((temporaryFullOutputBuffer[j++]!=1)
			||(temporaryFullOutputBuffer[j++]!=1)
			||(temporaryFullOutputBuffer[j++]!=1)
			||(temporaryFullOutputBuffer[j++]!=1)){
					LOG_INFO("WRONG WRONG WRONG");
			
		}
		if((temporaryFullOutputBuffer[j++]!=0)
			||(temporaryFullOutputBuffer[j++]!=0)
			||(temporaryFullOutputBuffer[j++]!=0)
			||(temporaryFullOutputBuffer[j++]!=0)){
					LOG_INFO("WRONG WRONG WRONG");
			
		}
		l++;
	}*/
/*	std::string binaryStringActualOrderStored(_fullOutputBufferLength, 'z');
	for (size_t i = 0; i < _fullOutputBufferLength; i++)
	{
		if (temporaryFullOutputBuffer[i])
			binaryStringActualOrderStored[i] = '1';
		else{
			binaryStringActualOrderStored[i] = '0';
		}
	}
	LOG_INFO("what came out in order it came out from left ot right: %s", binaryStringActualOrderStored.c_str());*/
}

void FPGAInterface::sleep()
{
    Delay::usBlockingOtherTasks(_sleepUs);
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

	bool* temporaryInputBuffer = new bool[_inputsLength]();
	bool* temporaryFullOutputBuffer = new bool[_fullOutputBufferLength]();
    while (true)
    {
		int64_t startTime = TimeHelper::ms();
		if (_disposed.load()) {
			_taskFinished = true;
			return;
		}
		uint64_t timeUs = TimeHelper::us();
		_lockInputBuffer.lock();
		if (!_inputsChanged)
		{
			_lockInputBuffer.unlock();
			readOutputs(false, temporaryFullOutputBuffer);
		}
		else
		{
			for(size_t i=0; i<_inputsLength; i++){
				temporaryInputBuffer[i]=_inputBuffer[i];
			}
			_inputsChanged = false;
			_lockInputBuffer.unlock();
			shiftValuesIn(temporaryInputBuffer);
			readOutputs(true, temporaryFullOutputBuffer);
			if (validateStagedInputs(temporaryInputBuffer, temporaryFullOutputBuffer))
			{
				setStagedInputsLive();
			}
			else{
				std::lock_guard<std::mutex> guard(_lockInputBuffer);
				_inputsChanged = true;
			}
		}
		{
			std::lock_guard<std::mutex> guard(_lockFullOutputsBuffer);
			for(size_t i=0; i<_fullOutputBufferLength; i++){
				_fullOutputBuffer[i]=temporaryFullOutputBuffer[i];
			}
		}
		_lastUpdateTimeUs.store(timeUs, std::memory_order_relaxed);
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
