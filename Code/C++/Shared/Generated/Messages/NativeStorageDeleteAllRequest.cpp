#include "./NativeStorageDeleteAllRequest.hpp"
const char* NativeStorageDeleteAllRequest::TYPE = "nsda";
NativeStorageDeleteAllRequest::NativeStorageDeleteAllRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeStorageDeleteAllRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativeStorageDeleteAllRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeStorageDeleteAllRequest> NativeStorageDeleteAllRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<NativeStorageDeleteAllRequest>(ticket);
}
NativeStorageDeleteAllRequest::~NativeStorageDeleteAllRequest(){
}
