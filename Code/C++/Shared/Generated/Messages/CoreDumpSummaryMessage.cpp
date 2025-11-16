#include "./CoreDumpSummaryMessage.hpp"
const char* CoreDumpSummaryMessage::TYPE = "cd";
CoreDumpSummaryMessage::CoreDumpSummaryMessage(
    uint32_t* aRegisterSetWhenTheExceptionCaused, size_t aRegisterSetWhenTheExceptionCausedLength, 
    uint32_t* backtrace, size_t backtraceLength, 
    bool backtraceCorrupted, 
    uint8_t bitMaskOfAvailableEPCxRegisters, 
    uint32_t causeOfException, 
    const char* crashingApplicationsSHA256SumAsAString, 
    uint32_t* pCRegisterAddressAtExceptionLevel1To7, size_t pCRegisterAddressAtExceptionLevel1To7Length, 
    uint32_t programCounterForException, 
    const char* taskName, 
    uint32_t taskPointer, 
    uint32_t version, 
    uint32_t virtualAddressOfException):
        _aRegisterSetWhenTheExceptionCaused(aRegisterSetWhenTheExceptionCaused),
        _aRegisterSetWhenTheExceptionCausedLength(aRegisterSetWhenTheExceptionCausedLength),
        _backtrace(backtrace),
        _backtraceLength(backtraceLength),
        _backtraceCorrupted(backtraceCorrupted),
        _bitMaskOfAvailableEPCxRegisters(bitMaskOfAvailableEPCxRegisters),
        _causeOfException(causeOfException),
        _crashingApplicationsSHA256SumAsAString(crashingApplicationsSHA256SumAsAString),
        _pCRegisterAddressAtExceptionLevel1To7(pCRegisterAddressAtExceptionLevel1To7),
        _pCRegisterAddressAtExceptionLevel1To7Length(pCRegisterAddressAtExceptionLevel1To7Length),
        _programCounterForException(programCounterForException),
        _taskName(taskName),
        _taskPointer(taskPointer),
        _version(version),
        _virtualAddressOfException(virtualAddressOfException),
        _freeMemoryInDeconstructor(false){
}
uint32_t* CoreDumpSummaryMessage::getARegisterSetWhenTheExceptionCaused(size_t& length){
    length = _aRegisterSetWhenTheExceptionCausedLength;
    return this->_aRegisterSetWhenTheExceptionCaused;
}
uint32_t* CoreDumpSummaryMessage::getBacktrace(size_t& length){
    length = _backtraceLength;
    return this->_backtrace;
}
bool CoreDumpSummaryMessage::getBacktraceCorrupted(){
    return this->_backtraceCorrupted;
}
uint8_t CoreDumpSummaryMessage::getBitMaskOfAvailableEPCxRegisters(){
    return this->_bitMaskOfAvailableEPCxRegisters;
}
uint32_t CoreDumpSummaryMessage::getCauseOfException(){
    return this->_causeOfException;
}
const char* CoreDumpSummaryMessage::getCrashingApplicationsSHA256SumAsAString(){
    return this->_crashingApplicationsSHA256SumAsAString;
}
uint32_t* CoreDumpSummaryMessage::getPCRegisterAddressAtExceptionLevel1To7(size_t& length){
    length = _pCRegisterAddressAtExceptionLevel1To7Length;
    return this->_pCRegisterAddressAtExceptionLevel1To7;
}
uint32_t CoreDumpSummaryMessage::getProgramCounterForException(){
    return this->_programCounterForException;
}
const char* CoreDumpSummaryMessage::getTaskName(){
    return this->_taskName;
}
uint32_t CoreDumpSummaryMessage::getTaskPointer(){
    return this->_taskPointer;
}
uint32_t CoreDumpSummaryMessage::getVersion(){
    return this->_version;
}
uint32_t CoreDumpSummaryMessage::getVirtualAddressOfException(){
    return this->_virtualAddressOfException;
}
cJSON* CoreDumpSummaryMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addArray<uint32_t>(j, "areg", _aRegisterSetWhenTheExceptionCaused, _aRegisterSetWhenTheExceptionCausedLength);
    JHelper::addArray<uint32_t>(j, "b", _backtrace, _backtraceLength);
    JHelper::addBool(j, "bcpt", this->_backtraceCorrupted);
    JHelper::addUInt8(j, "bitm", this->_bitMaskOfAvailableEPCxRegisters);
    JHelper::addUInt32(j, "coe", this->_causeOfException);
    JHelper::addString(j, "casha", this->_crashingApplicationsSHA256SumAsAString);
    JHelper::addArray<uint32_t>(j, "pcreg", _pCRegisterAddressAtExceptionLevel1To7, _pCRegisterAddressAtExceptionLevel1To7Length);
    JHelper::addUInt32(j, "pc", this->_programCounterForException);
    JHelper::addString(j, "tn", this->_taskName);
    JHelper::addUInt32(j, "tp", this->_taskPointer);
    JHelper::addUInt32(j, "v", this->_version);
    JHelper::addUInt32(j, "va", this->_virtualAddressOfException);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<CoreDumpSummaryMessage> CoreDumpSummaryMessage::fromJSON(cJSON* j){
    bool s = true;
    size_t aRegisterSetWhenTheExceptionCausedLength;
    uint32_t* aRegisterSetWhenTheExceptionCaused = JHelper::getArray<uint32_t>(j, "areg", aRegisterSetWhenTheExceptionCausedLength);
    size_t backtraceLength;
    uint32_t* backtrace = JHelper::getArray<uint32_t>(j, "b", backtraceLength);
    bool backtraceCorrupted = JHelper::getBool(j, "bcpt", s);
    uint8_t bitMaskOfAvailableEPCxRegisters = JHelper::getUInt8(j, "bitm", s);
    uint32_t causeOfException = JHelper::getUInt32(j, "coe", s);
    const char* crashingApplicationsSHA256SumAsAString = JHelper::getString(j, "casha", s);
    size_t pCRegisterAddressAtExceptionLevel1To7Length;
    uint32_t* pCRegisterAddressAtExceptionLevel1To7 = JHelper::getArray<uint32_t>(j, "pcreg", pCRegisterAddressAtExceptionLevel1To7Length);
    uint32_t programCounterForException = JHelper::getUInt32(j, "pc", s);
    const char* taskName = JHelper::getString(j, "tn", s);
    uint32_t taskPointer = JHelper::getUInt32(j, "tp", s);
    uint32_t version = JHelper::getUInt32(j, "v", s);
    uint32_t virtualAddressOfException = JHelper::getUInt32(j, "va", s);
    auto r = std::make_shared<CoreDumpSummaryMessage>(aRegisterSetWhenTheExceptionCaused, aRegisterSetWhenTheExceptionCausedLength, backtrace, backtraceLength, backtraceCorrupted, bitMaskOfAvailableEPCxRegisters, causeOfException, crashingApplicationsSHA256SumAsAString, pCRegisterAddressAtExceptionLevel1To7, pCRegisterAddressAtExceptionLevel1To7Length, programCounterForException, taskName, taskPointer, version, virtualAddressOfException);
r->_freeMemoryInDeconstructor = true;
return r;
}
CoreDumpSummaryMessage::~CoreDumpSummaryMessage(){
if(!_freeMemoryInDeconstructor)return;
     if(_aRegisterSetWhenTheExceptionCaused!=nullptr)delete[] _aRegisterSetWhenTheExceptionCaused;
     if(_backtrace!=nullptr)delete[] _backtrace;
     if(_crashingApplicationsSHA256SumAsAString!=nullptr)delete[] _crashingApplicationsSHA256SumAsAString;
     if(_pCRegisterAddressAtExceptionLevel1To7!=nullptr)delete[] _pCRegisterAddressAtExceptionLevel1To7;
     if(_taskName!=nullptr)delete[] _taskName;
}
