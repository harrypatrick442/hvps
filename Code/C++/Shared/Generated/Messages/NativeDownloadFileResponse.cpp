#include "./NativeDownloadFileResponse.hpp"
const char* NativeDownloadFileResponse::TYPE = "tkd";
NativeDownloadFileResponse::NativeDownloadFileResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeDownloadFileResponse::getTicket(){
    return this->_ticket;
}
cJSON* NativeDownloadFileResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeDownloadFileResponse> NativeDownloadFileResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativeDownloadFileResponse>(ticket);
}
NativeDownloadFileResponse::~NativeDownloadFileResponse(){
}
