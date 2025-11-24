#include "./HasBusMessage.hpp"
const char* HasBusMessage::TYPE = "hb";
HasBusMessage::HasBusMessage(
    int32_t target):
        _target(target){
}
int32_t HasBusMessage::getTarget()const noexcept{
    return this->_target;
}
cJSON* HasBusMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "tgt", this->_target);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
HasBusMessage* HasBusMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t target = JHelper::getInt32(j, "tgt", s);
    auto r = new HasBusMessage(target);
    cleanupBucket.addDelete(r);
    return r;
}
HasBusMessage::~HasBusMessage(){
}
