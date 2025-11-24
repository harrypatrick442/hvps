#include "./IndicateStateMessage.hpp"
const char* IndicateStateMessage::TYPE = "is";
IndicateStateMessage::IndicateStateMessage(
    int32_t systemState):
        _systemState(systemState){
}
int32_t IndicateStateMessage::getSystemState()const noexcept{
    return this->_systemState;
}
cJSON* IndicateStateMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "s", this->_systemState);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
IndicateStateMessage* IndicateStateMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t systemState = JHelper::getInt32(j, "s", s);
    auto r = new IndicateStateMessage(systemState);
    cleanupBucket.addDelete(r);
    return r;
}
IndicateStateMessage::~IndicateStateMessage(){
}
