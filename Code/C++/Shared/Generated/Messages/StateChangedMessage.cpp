#include "./StateChangedMessage.hpp"
const char* StateChangedMessage::TYPE = "sc";
StateChangedMessage::StateChangedMessage(
    int32_t state):
        _state(state){
}
int32_t StateChangedMessage::getState(){
    return this->_state;
}
cJSON* StateChangedMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "s", this->_state);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<StateChangedMessage> StateChangedMessage::fromJSON(cJSON* j){
    bool s = true;
    int32_t state = JHelper::getInt32(j, "s", s);
    auto r = std::make_shared<StateChangedMessage>(state);
return r;
}
StateChangedMessage::~StateChangedMessage(){
}
