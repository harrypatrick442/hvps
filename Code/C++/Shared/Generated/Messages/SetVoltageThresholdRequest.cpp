#include "./SetVoltageThresholdRequest.hpp"
const char* SetVoltageThresholdRequest::TYPE = "svf";
SetVoltageThresholdRequest::SetVoltageThresholdRequest(
    double voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
double SetVoltageThresholdRequest::getVoltage()const noexcept{
    return this->_voltage;
}
uint64_t SetVoltageThresholdRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* SetVoltageThresholdRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
SetVoltageThresholdRequest* SetVoltageThresholdRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new SetVoltageThresholdRequest(voltage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
SetVoltageThresholdRequest::~SetVoltageThresholdRequest(){
}
