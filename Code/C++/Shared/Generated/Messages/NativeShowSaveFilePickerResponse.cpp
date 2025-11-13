#include "./NativeShowSaveFilePickerResponse.hpp"
const char* NativeShowSaveFilePickerResponse::TYPE = "tkd";
NativeShowSaveFilePickerResponse::NativeShowSaveFilePickerResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeShowSaveFilePickerResponse::getTicket(){
    return this->_ticket;
}
cJSON* NativeShowSaveFilePickerResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeShowSaveFilePickerResponse> NativeShowSaveFilePickerResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<NativeShowSaveFilePickerResponse>(ticket);
return r;
}
NativeShowSaveFilePickerResponse::~NativeShowSaveFilePickerResponse(){
}
