#include "./NativeOpenDirectoryRequest.hpp"
const char* NativeOpenDirectoryRequest::TYPE = "nod";
NativeOpenDirectoryRequest::NativeOpenDirectoryRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeOpenDirectoryRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeOpenDirectoryRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeOpenDirectoryRequest* NativeOpenDirectoryRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeOpenDirectoryRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeOpenDirectoryRequest::~NativeOpenDirectoryRequest(){
}
