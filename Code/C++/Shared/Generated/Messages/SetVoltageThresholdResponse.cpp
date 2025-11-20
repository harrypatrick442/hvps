#include "./SetVoltageThresholdResponse.hpp"
const char* SetVoltageThresholdResponse::TYPE = "tkd";
SetVoltageThresholdResponse::SetVoltageThresholdResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t SetVoltageThresholdResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* SetVoltageThresholdResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
SetVoltageThresholdResponse* SetVoltageThresholdResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new SetVoltageThresholdResponse(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
SetVoltageThresholdResponse::~SetVoltageThresholdResponse(){
}
