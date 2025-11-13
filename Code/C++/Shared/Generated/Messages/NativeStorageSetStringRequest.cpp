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
const char* NativeStorageSetStringRequest::getKey(){
    return this->_key;
}
const char* NativeStorageSetStringRequest::getValue(){
    return this->_value;
}
uint64_t NativeStorageSetStringRequest::getTicket(){
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
std::shared_ptr<NativeStorageSetStringRequest> NativeStorageSetStringRequest::fromJSON(cJSON* j){
    bool s = true;
    const char* key = JHelper::getString(j, "k", s);
    const char* value = JHelper::getString(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<NativeStorageSetStringRequest>(key, value, ticket);
return r;
}
NativeStorageSetStringRequest::~NativeStorageSetStringRequest(){
}
