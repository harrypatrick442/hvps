#include "./GetVoltageThresholdRequest.hpp"
const char* GetVoltageThresholdRequest::TYPE = "gvf";
GetVoltageThresholdRequest::GetVoltageThresholdRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetVoltageThresholdRequest::getTicket(){
    return this->_ticket;
}
cJSON* GetVoltageThresholdRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetVoltageThresholdRequest> GetVoltageThresholdRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<GetVoltageThresholdRequest>(ticket);
return r;
}
GetVoltageThresholdRequest::~GetVoltageThresholdRequest(){
}
