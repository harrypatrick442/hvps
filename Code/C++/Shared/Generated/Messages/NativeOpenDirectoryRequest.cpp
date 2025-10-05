#include "./NativeOpenDirectoryRequest.hpp"
const char* NativeOpenDirectoryRequest::TYPE = "nod";
NativeOpenDirectoryRequest::NativeOpenDirectoryRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeOpenDirectoryRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativeOpenDirectoryRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeOpenDirectoryRequest> NativeOpenDirectoryRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativeOpenDirectoryRequest>(ticket);
}
NativeOpenDirectoryRequest::~NativeOpenDirectoryRequest(){
}
