#include "./NativeStorageDeleteAllResponse.hpp"
const char* NativeStorageDeleteAllResponse::TYPE = "tkd";
NativeStorageDeleteAllResponse::NativeStorageDeleteAllResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageDeleteAllResponse::getTicket(){
    return this->_ticket;
}
cJSON* NativeStorageDeleteAllResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeStorageDeleteAllResponse> NativeStorageDeleteAllResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<NativeStorageDeleteAllResponse>(ticket);
return r;
}
NativeStorageDeleteAllResponse::~NativeStorageDeleteAllResponse(){
}
