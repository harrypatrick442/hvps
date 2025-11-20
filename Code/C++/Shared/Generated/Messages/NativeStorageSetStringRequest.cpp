#include "./NativeStorageSetStringRequest.hpp"
const char* NativeStorageSetStringRequest::TYPE = "nsss";
NativeStorageSetStringRequest::NativeStorageSetStringRequest(
    const char* key, 
    const char* value, 
    uint64_t ticket):
        _key(key),
        _value(value),
        _ticket(ticket){
}
const char* NativeStorageSetStringRequest::getKey()const noexcept{
    return this->_key;
}
const char* NativeStorageSetStringRequest::getValue()const noexcept{
    return this->_value;
}
uint64_t NativeStorageSetStringRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageSetStringRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "k", this->_key);
    JHelper::addString(j, "v", this->_value);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageSetStringRequest* NativeStorageSetStringRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* key = JHelper::getString(j, "k", s);
    cleanupBucket.addDeleteArray(key);
    const char* value = JHelper::getString(j, "v", s);
    cleanupBucket.addDeleteArray(value);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageSetStringRequest(key, value, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageSetStringRequest::~NativeStorageSetStringRequest(){
}
