#include "./NativeStorageGetStringRequest.hpp"
const char* NativeStorageGetStringRequest::TYPE = "nsgs";
NativeStorageGetStringRequest::NativeStorageGetStringRequest(
    const char* key, 
    uint64_t ticket):
        _key(key),
        _ticket(ticket){
}
const char* NativeStorageGetStringRequest::getKey()const noexcept{
    return this->_key;
}
uint64_t NativeStorageGetStringRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageGetStringRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "k", this->_key);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageGetStringRequest* NativeStorageGetStringRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* key = JHelper::getString(j, "k", s);
    cleanupBucket.addDeleteArray(key);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageGetStringRequest(key, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageGetStringRequest::~NativeStorageGetStringRequest(){
}
