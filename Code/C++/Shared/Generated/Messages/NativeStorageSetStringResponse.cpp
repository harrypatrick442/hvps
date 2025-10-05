#include "./NativeStorageSetStringResponse.hpp"
const char* NativeStorageSetStringResponse::TYPE = "tkd";
NativeStorageSetStringResponse::NativeStorageSetStringResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageSetStringResponse::getTicket(){
    return this->_ticket;
}
cJSON* NativeStorageSetStringResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeStorageSetStringResponse> NativeStorageSetStringResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativeStorageSetStringResponse>(ticket);
}
NativeStorageSetStringResponse::~NativeStorageSetStringResponse(){
}
