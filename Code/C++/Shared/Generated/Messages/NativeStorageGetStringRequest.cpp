#include "./NativeStorageGetStringRequest.hpp"
const char* NativeStorageGetStringRequest::TYPE = "nsgs";
NativeStorageGetStringRequest::NativeStorageGetStringRequest(
    const char* key, 
    uint64_t ticket):
        _key(key),
        _ticket(ticket),
        _freeMemoryInDeconstructor(false){
}
const char* NativeStorageGetStringRequest::getKey(){
    return this->_key;
}
uint64_t NativeStorageGetStringRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativeStorageGetStringRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "k", this->_key);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeStorageGetStringRequest> NativeStorageGetStringRequest::fromJSON(cJSON* j){
    bool s = true;
    const char* key = JHelper::getString(j, "k", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<NativeStorageGetStringRequest>(key, ticket);
r->_freeMemoryInDeconstructor = true;
return r;
}
NativeStorageGetStringRequest::~NativeStorageGetStringRequest(){
if(!_freeMemoryInDeconstructor)return;
     if(_key!=nullptr)delete[] _key;
}
