#include "./NativeStorageSetStringResponse.hpp"
const char* NativeStorageSetStringResponse::TYPE = "tkd";
NativeStorageSetStringResponse::NativeStorageSetStringResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageSetStringResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeStorageSetStringResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeStorageSetStringResponse* NativeStorageSetStringResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeStorageSetStringResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeStorageSetStringResponse::~NativeStorageSetStringResponse(){
}
