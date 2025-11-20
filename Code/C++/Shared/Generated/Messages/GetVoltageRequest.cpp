#include "./GetVoltageRequest.hpp"
const char* GetVoltageRequest::TYPE = "gv";
GetVoltageRequest::GetVoltageRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GetVoltageRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetVoltageRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetVoltageRequest* GetVoltageRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetVoltageRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetVoltageRequest::~GetVoltageRequest(){
}
