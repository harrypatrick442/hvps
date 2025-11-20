#include "./NativePickFileResponse.hpp"
const char* NativePickFileResponse::TYPE = "tkd";
NativePickFileResponse::NativePickFileResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativePickFileResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativePickFileResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativePickFileResponse* NativePickFileResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativePickFileResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativePickFileResponse::~NativePickFileResponse(){
}
