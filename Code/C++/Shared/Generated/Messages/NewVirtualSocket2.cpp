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
int64_t NewVirtualSocket2::getEndpointId()const noexcept{
    return this->_endpointId;
}
const char* NewVirtualSocket2::getSecret()const noexcept{
    return this->_secret;
}
int64_t NewVirtualSocket2::getTheirNodeId()const noexcept{
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
NewVirtualSocket2* NewVirtualSocket2::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int64_t endpointId = JHelper::getInt64(j, "i", s);
    const char* secret = JHelper::getString(j, "s", s);
    cleanupBucket.addDeleteArray(secret);
    int64_t theirNodeId = JHelper::getInt64(j, "n", s);
    auto r = new NewVirtualSocket2(endpointId, secret, theirNodeId);
    cleanupBucket.addDelete(r);
    return r;
}
NewVirtualSocket2::~NewVirtualSocket2(){
}
