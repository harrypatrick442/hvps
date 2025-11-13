#include "./NativeShowSaveFilePickerRequest.hpp"
const char* NativeShowSaveFilePickerRequest::TYPE = "nsaf";
NativeShowSaveFilePickerRequest::NativeShowSaveFilePickerRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t NativeShowSaveFilePickerRequest::getTicket(){
    return this->_ticket;
}
cJSON* NativeShowSaveFilePickerRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativeShowSaveFilePickerRequest> NativeShowSaveFilePickerRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<NativeShowSaveFilePickerRequest>(ticket);
return r;
}
NativeShowSaveFilePickerRequest::~NativeShowSaveFilePickerRequest(){
}
