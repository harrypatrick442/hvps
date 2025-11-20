#include "./NativeDownloadFileRequest.hpp"
const char* NativeDownloadFileRequest::TYPE = "ndf";
NativeDownloadFileRequest::NativeDownloadFileRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeDownloadFileRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeDownloadFileRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeDownloadFileRequest* NativeDownloadFileRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeDownloadFileRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeDownloadFileRequest::~NativeDownloadFileRequest(){
}
