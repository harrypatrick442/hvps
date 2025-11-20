#include "./LastAbortMessage.hpp"
const char* LastAbortMessage::TYPE = "la";
LastAbortMessage::LastAbortMessage(
    uint32_t* backtrace, size_t backtraceLength, 
    const char* reason, 
    int32_t subsystemIdentifier):
        _backtrace(backtrace),
        _backtraceLength(backtraceLength),
        _reason(reason),
        _subsystemIdentifier(subsystemIdentifier){
}
uint32_t* LastAbortMessage::getBacktrace(size_t& length){
    length = _backtraceLength;
    return this->_backtrace;
}
const char* LastAbortMessage::getReason()const noexcept{
    return this->_reason;
}
int32_t LastAbortMessage::getSubsystemIdentifier()const noexcept{
    return this->_subsystemIdentifier;
}
cJSON* LastAbortMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addArray<uint32_t>(j, "bt", _backtrace, _backtraceLength);
    JHelper::addString(j, "r", this->_reason);
    JHelper::addInt32(j, "si", this->_subsystemIdentifier);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
LastAbortMessage* LastAbortMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    size_t backtraceLength;
    uint32_t* backtrace = JHelper::getArray<uint32_t>(j, "bt", backtraceLength);
    cleanupBucket.addDeleteArray(backtrace);
    const char* reason = JHelper::getString(j, "r", s);
    cleanupBucket.addDeleteArray(reason);
    int32_t subsystemIdentifier = JHelper::getInt32(j, "si", s);
    auto r = new LastAbortMessage(backtrace, backtraceLength, reason, subsystemIdentifier);
    cleanupBucket.addDelete(r);
    return r;
}
LastAbortMessage::~LastAbortMessage(){
}
