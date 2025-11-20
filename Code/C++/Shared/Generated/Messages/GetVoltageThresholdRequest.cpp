#include "./GetVoltageThresholdRequest.hpp"
const char* GetVoltageThresholdRequest::TYPE = "gvf";
GetVoltageThresholdRequest::GetVoltageThresholdRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetVoltageThresholdRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetVoltageThresholdRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetVoltageThresholdRequest* GetVoltageThresholdRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetVoltageThresholdRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetVoltageThresholdRequest::~GetVoltageThresholdRequest(){
}
