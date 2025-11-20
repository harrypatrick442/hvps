#include "./NativeDownloadFileResponse.hpp"
const char* NativeDownloadFileResponse::TYPE = "tkd";
NativeDownloadFileResponse::NativeDownloadFileResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeDownloadFileResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeDownloadFileResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeDownloadFileResponse* NativeDownloadFileResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeDownloadFileResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeDownloadFileResponse::~NativeDownloadFileResponse(){
}
