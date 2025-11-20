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
    int32_t subsystemIdentifier, 
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
        _subsystemIdentifier(subsystemIdentifier),
        _taskName(taskName),
        _taskPointer(taskPointer),
        _version(version),
        _virtualAddressOfException(virtualAddressOfException){
}
uint32_t* CoreDumpSummaryMessage::getARegisterSetWhenTheExceptionCaused(size_t& length){
    length = _aRegisterSetWhenTheExceptionCausedLength;
    return this->_aRegisterSetWhenTheExceptionCaused;
}
uint32_t* CoreDumpSummaryMessage::getBacktrace(size_t& length){
    length = _backtraceLength;
    return this->_backtrace;
}
bool CoreDumpSummaryMessage::getBacktraceCorrupted()const noexcept{
    return this->_backtraceCorrupted;
}
uint8_t CoreDumpSummaryMessage::getBitMaskOfAvailableEPCxRegisters()const noexcept{
    return this->_bitMaskOfAvailableEPCxRegisters;
}
uint32_t CoreDumpSummaryMessage::getCauseOfException()const noexcept{
    return this->_causeOfException;
}
const char* CoreDumpSummaryMessage::getCrashingApplicationsSHA256SumAsAString()const noexcept{
    return this->_crashingApplicationsSHA256SumAsAString;
}
uint32_t* CoreDumpSummaryMessage::getPCRegisterAddressAtExceptionLevel1To7(size_t& length){
    length = _pCRegisterAddressAtExceptionLevel1To7Length;
    return this->_pCRegisterAddressAtExceptionLevel1To7;
}
uint32_t CoreDumpSummaryMessage::getProgramCounterForException()const noexcept{
    return this->_programCounterForException;
}
int32_t CoreDumpSummaryMessage::getSubsystemIdentifier()const noexcept{
    return this->_subsystemIdentifier;
}
const char* CoreDumpSummaryMessage::getTaskName()const noexcept{
    return this->_taskName;
}
uint32_t CoreDumpSummaryMessage::getTaskPointer()const noexcept{
    return this->_taskPointer;
}
uint32_t CoreDumpSummaryMessage::getVersion()const noexcept{
    return this->_version;
}
uint32_t CoreDumpSummaryMessage::getVirtualAddressOfException()const noexcept{
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
    JHelper::addInt32(j, "si", this->_subsystemIdentifier);
    JHelper::addString(j, "tn", this->_taskName);
    JHelper::addUInt32(j, "tp", this->_taskPointer);
    JHelper::addUInt32(j, "v", this->_version);
    JHelper::addUInt32(j, "va", this->_virtualAddressOfException);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
CoreDumpSummaryMessage* CoreDumpSummaryMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    size_t aRegisterSetWhenTheExceptionCausedLength;
    uint32_t* aRegisterSetWhenTheExceptionCaused = JHelper::getArray<uint32_t>(j, "areg", aRegisterSetWhenTheExceptionCausedLength);
    cleanupBucket.addDeleteArray(aRegisterSetWhenTheExceptionCaused);
    size_t backtraceLength;
    uint32_t* backtrace = JHelper::getArray<uint32_t>(j, "b", backtraceLength);
    cleanupBucket.addDeleteArray(backtrace);
    bool backtraceCorrupted = JHelper::getBool(j, "bcpt", s);
    uint8_t bitMaskOfAvailableEPCxRegisters = JHelper::getUInt8(j, "bitm", s);
    uint32_t causeOfException = JHelper::getUInt32(j, "coe", s);
    const char* crashingApplicationsSHA256SumAsAString = JHelper::getString(j, "casha", s);
    cleanupBucket.addDeleteArray(crashingApplicationsSHA256SumAsAString);
    size_t pCRegisterAddressAtExceptionLevel1To7Length;
    uint32_t* pCRegisterAddressAtExceptionLevel1To7 = JHelper::getArray<uint32_t>(j, "pcreg", pCRegisterAddressAtExceptionLevel1To7Length);
    cleanupBucket.addDeleteArray(pCRegisterAddressAtExceptionLevel1To7);
    uint32_t programCounterForException = JHelper::getUInt32(j, "pc", s);
    int32_t subsystemIdentifier = JHelper::getInt32(j, "si", s);
    const char* taskName = JHelper::getString(j, "tn", s);
    cleanupBucket.addDeleteArray(taskName);
    uint32_t taskPointer = JHelper::getUInt32(j, "tp", s);
    uint32_t version = JHelper::getUInt32(j, "v", s);
    uint32_t virtualAddressOfException = JHelper::getUInt32(j, "va", s);
    auto r = new CoreDumpSummaryMessage(aRegisterSetWhenTheExceptionCaused, aRegisterSetWhenTheExceptionCausedLength, backtrace, backtraceLength, backtraceCorrupted, bitMaskOfAvailableEPCxRegisters, causeOfException, crashingApplicationsSHA256SumAsAString, pCRegisterAddressAtExceptionLevel1To7, pCRegisterAddressAtExceptionLevel1To7Length, programCounterForException, subsystemIdentifier, taskName, taskPointer, version, virtualAddressOfException);
    cleanupBucket.addDelete(r);
    return r;
}
CoreDumpSummaryMessage::~CoreDumpSummaryMessage(){
}
