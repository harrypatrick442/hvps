#include "./HVPSStateChangedMessage.hpp"
const char* HVPSStateChangedMessage::TYPE = "sc";
HVPSStateChangedMessage::HVPSStateChangedMessage(
    int32_t state):
        _state(state){
}
int32_t HVPSStateChangedMessage::getState(){
    return this->_state;
}
cJSON* HVPSStateChangedMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "s", this->_state);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSStateChangedMessage> HVPSStateChangedMessage::fromJSON(cJSON* j){
    bool s = true;
    int32_t state = JHelper::getInt32(j, "s", s);
    return std::make_shared<HVPSStateChangedMessage>(state);
}
HVPSStateChangedMessage::~HVPSStateChangedMessage(){
}
