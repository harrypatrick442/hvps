#include "./NativePickFileRequest.hpp"
const char* NativePickFileRequest::TYPE = "npf";
NativePickFileRequest::NativePickFileRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativePickFileRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativePickFileRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativePickFileRequest* NativePickFileRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativePickFileRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativePickFileRequest::~NativePickFileRequest(){
}
