#include "./StateChangedMessage.hpp"
const char* StateChangedMessage::TYPE = "sc";
StateChangedMessage::StateChangedMessage(
    int32_t state):
        _state(state){
}
int32_t StateChangedMessage::getState()const noexcept{
    return this->_state;
}
cJSON* StateChangedMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "s", this->_state);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
StateChangedMessage* StateChangedMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t state = JHelper::getInt32(j, "s", s);
    auto r = new StateChangedMessage(state);
    cleanupBucket.addDelete(r);
    return r;
}
StateChangedMessage::~StateChangedMessage(){
}
