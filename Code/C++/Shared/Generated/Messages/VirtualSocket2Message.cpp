#include "./VirtualSocket2Message.hpp"
const char* VirtualSocket2Message::TYPE = "vs3m";
VirtualSocket2Message::VirtualSocket2Message(
    std::optional<int64_t> endpointId, 
    const char* payload, 
    const char* secret, 
    std::optional<int32_t> theirNodeId):
        _endpointId(endpointId),
        _payload(payload),
        _secret(secret),
        _theirNodeId(theirNodeId){
}
std::optional<int64_t> VirtualSocket2Message::getEndpointId(){
    return this->_endpointId;
}
const char* VirtualSocket2Message::getPayload(){
    return this->_payload;
}
const char* VirtualSocket2Message::getSecret(){
    return this->_secret;
}
std::optional<int32_t> VirtualSocket2Message::getTheirNodeId(){
    return this->_theirNodeId;
}
cJSON* VirtualSocket2Message::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableInt64(j, "i", this->_endpointId);
    JHelper::addString(j, "p", this->_payload);
    JHelper::addString(j, "s", this->_secret);
    JHelper::addNullableInt32(j, "n", this->_theirNodeId);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<VirtualSocket2Message> VirtualSocket2Message::fromJSON(cJSON* j){
    bool s = true;
    std::optional<int64_t> endpointId = JHelper::getNullableInt64(j, "i", s);
    const char* payload = JHelper::getString(j, "p", s);
    const char* secret = JHelper::getString(j, "s", s);
    std::optional<int32_t> theirNodeId = JHelper::getNullableInt32(j, "n", s);
    auto r = std::make_shared<VirtualSocket2Message>(endpointId, payload, secret, theirNodeId);
return r;
}
VirtualSocket2Message::~VirtualSocket2Message(){
}
