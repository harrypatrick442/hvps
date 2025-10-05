#include "./NativePickFileRequest.hpp"
const char* NativePickFileRequest::TYPE = "npf";
NativePickFileRequest::NativePickFileRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativePickFileRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativePickFileRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativePickFileRequest> NativePickFileRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativePickFileRequest>(ticket);
}
NativePickFileRequest::~NativePickFileRequest(){
}
