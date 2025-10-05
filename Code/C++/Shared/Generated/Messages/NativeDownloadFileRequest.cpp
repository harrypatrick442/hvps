#include "./NativeDownloadFileRequest.hpp"
const char* NativeDownloadFileRequest::TYPE = "ndf";
NativeDownloadFileRequest::NativeDownloadFileRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeDownloadFileRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativeDownloadFileRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeDownloadFileRequest> NativeDownloadFileRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativeDownloadFileRequest>(ticket);
}
NativeDownloadFileRequest::~NativeDownloadFileRequest(){
}
