#include "./VirtualSocketMessage.hpp"
const char* VirtualSocketMessage::TYPE = "v";
VirtualSocketMessage::VirtualSocketMessage(
    int64_t id, 
    const char* internalType, 
    const char* payload):
        _id(id),
        _internalType(internalType),
        _payload(payload){
}
int64_t VirtualSocketMessage::getId(){
    return this->_id;
}
const char* VirtualSocketMessage::getInternalType(){
    return this->_internalType;
}
const char* VirtualSocketMessage::getPayload(){
    return this->_payload;
}
cJSON* VirtualSocketMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt64(j, "i", this->_id);
    JHelper::addString(j, "u", this->_internalType);
    JHelper::addString(j, "p", this->_payload);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<VirtualSocketMessage> VirtualSocketMessage::fromJSON(cJSON* j){
    bool s = true;
    int64_t id = JHelper::getInt64(j, "i", s);
    const char* internalType = JHelper::getString(j, "u", s);
    const char* payload = JHelper::getString(j, "p", s);
    auto r = std::make_shared<VirtualSocketMessage>(id, internalType, payload);
return r;
}
VirtualSocketMessage::~VirtualSocketMessage(){
}
