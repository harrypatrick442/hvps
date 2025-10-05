#include "./NewVirtualSocket2.hpp"
const char* NewVirtualSocket2::TYPE = "vs3m";
NewVirtualSocket2::NewVirtualSocket2(
    int64_t endpointId, 
    const char* secret, 
    int64_t theirNodeId):
        _endpointId(endpointId),
        _secret(secret),
        _theirNodeId(theirNodeId){
}
int64_t NewVirtualSocket2::getEndpointId(){
    return this->_endpointId;
}
const char* NewVirtualSocket2::getSecret(){
    return this->_secret;
}
int64_t NewVirtualSocket2::getTheirNodeId(){
    return this->_theirNodeId;
}
cJSON* NewVirtualSocket2::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt64(j, "i", this->_endpointId);
    JHelper::addString(j, "s", this->_secret);
    JHelper::addInt64(j, "n", this->_theirNodeId);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NewVirtualSocket2> NewVirtualSocket2::fromJSON(cJSON* j){
    bool s = true;
    int64_t endpointId = JHelper::getInt64(j, "i", s);
    const char* secret = JHelper::getString(j, "s", s);
    int64_t theirNodeId = JHelper::getInt64(j, "n", s);
    return std::make_shared<NewVirtualSocket2>(endpointId, secret, theirNodeId);
}
NewVirtualSocket2::~NewVirtualSocket2(){
}
