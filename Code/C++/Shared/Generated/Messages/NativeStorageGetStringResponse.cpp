#include "./NativeStorageGetStringResponse.hpp"
const char* NativeStorageGetStringResponse::TYPE = "tkd";
NativeStorageGetStringResponse::NativeStorageGetStringResponse(
    const char* value, 
    uint64_t ticket):
        _value(value),
        _ticket(ticket){
}
const char* NativeStorageGetStringResponse::getValue()const noexcept{
    return this->_value;
}
uint64_t NativeStorageGetStringResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageGetStringResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "v", this->_value);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageGetStringResponse* NativeStorageGetStringResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* value = JHelper::getString(j, "v", s);
    cleanupBucket.addDeleteArray(value);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageGetStringResponse(value, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageGetStringResponse::~NativeStorageGetStringResponse(){
}
