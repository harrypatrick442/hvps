#include "./LastAbortMessage.hpp"
const char* LastAbortMessage::TYPE = "la";
LastAbortMessage::LastAbortMessage(
    uint32_t* backtrace, size_t backtraceLength, 
    const char* reason):
        _backtrace(backtrace),
        _backtraceLength(backtraceLength),
        _reason(reason),
        _freeMemoryInDeconstructor(false){
}
uint32_t* LastAbortMessage::getBacktrace(size_t& length){
    length = _backtraceLength;
    return this->_backtrace;
}
const char* LastAbortMessage::getReason(){
    return this->_reason;
}
cJSON* LastAbortMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addArray<uint32_t>(j, "bt", _backtrace, _backtraceLength);
    JHelper::addString(j, "r", this->_reason);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<LastAbortMessage> LastAbortMessage::fromJSON(cJSON* j){
    bool s = true;
    size_t backtraceLength;
    uint32_t* backtrace = JHelper::getArray<uint32_t>(j, "bt", backtraceLength);
    const char* reason = JHelper::getString(j, "r", s);
    auto r = std::make_shared<LastAbortMessage>(backtrace, backtraceLength, reason);
r->_freeMemoryInDeconstructor = true;
return r;
}
LastAbortMessage::~LastAbortMessage(){
if(!_freeMemoryInDeconstructor)return;
     if(_backtrace!=nullptr)delete[] _backtrace;
}
