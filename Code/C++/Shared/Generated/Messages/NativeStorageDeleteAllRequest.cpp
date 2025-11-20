#include "./NativeStorageDeleteAllRequest.hpp"
const char* NativeStorageDeleteAllRequest::TYPE = "nsda";
NativeStorageDeleteAllRequest::NativeStorageDeleteAllRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageDeleteAllRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageDeleteAllRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageDeleteAllRequest* NativeStorageDeleteAllRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageDeleteAllRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageDeleteAllRequest::~NativeStorageDeleteAllRequest(){
}
