#include "./NativePickFileResponse.hpp"
const char* NativePickFileResponse::TYPE = "tkd";
NativePickFileResponse::NativePickFileResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativePickFileResponse::getTicket(){
    return this->_ticket;
}
cJSON* NativePickFileResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativePickFileResponse> NativePickFileResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativePickFileResponse>(ticket);
}
NativePickFileResponse::~NativePickFileResponse(){
}
