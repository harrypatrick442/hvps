#include "./NativeStorageDeleteAllResponse.hpp"
const char* NativeStorageDeleteAllResponse::TYPE = "tkd";
NativeStorageDeleteAllResponse::NativeStorageDeleteAllResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageDeleteAllResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageDeleteAllResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageDeleteAllResponse* NativeStorageDeleteAllResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageDeleteAllResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageDeleteAllResponse::~NativeStorageDeleteAllResponse(){
}
