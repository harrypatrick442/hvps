#include "./LastAbortMessage.hpp"
const char* LastAbortMessage::TYPE = "la";
LastAbortMessage::LastAbortMessage(
    const char* reason):
        _reason(reason){
}
const char* LastAbortMessage::getReason(){
    return this->_reason;
}
cJSON* LastAbortMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "r", this->_reason);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<LastAbortMessage> LastAbortMessage::fromJSON(cJSON* j){
    bool s = true;
    const char* reason = JHelper::getString(j, "r", s);
    auto r = std::make_shared<LastAbortMessage>(reason);
return r;
}
LastAbortMessage::~LastAbortMessage(){
}
