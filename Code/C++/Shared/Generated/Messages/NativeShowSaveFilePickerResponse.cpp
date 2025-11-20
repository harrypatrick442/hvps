#include "./NativeShowSaveFilePickerResponse.hpp"
const char* NativeShowSaveFilePickerResponse::TYPE = "tkd";
NativeShowSaveFilePickerResponse::NativeShowSaveFilePickerResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeShowSaveFilePickerResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* NativeShowSaveFilePickerResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeShowSaveFilePickerResponse* NativeShowSaveFilePickerResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new NativeShowSaveFilePickerResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
NativeShowSaveFilePickerResponse::~NativeShowSaveFilePickerResponse(){
}
