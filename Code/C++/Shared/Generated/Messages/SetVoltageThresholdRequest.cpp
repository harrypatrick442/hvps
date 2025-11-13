#include "./SetVoltageThresholdRequest.hpp"
const char* SetVoltageThresholdRequest::TYPE = "svf";
SetVoltageThresholdRequest::SetVoltageThresholdRequest(
    double voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
double SetVoltageThresholdRequest::getVoltage(){
    return this->_voltage;
}
uint64_t SetVoltageThresholdRequest::getTicket(){
    return this->_ticket;
}
cJSON* SetVoltageThresholdRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<SetVoltageThresholdRequest> SetVoltageThresholdRequest::fromJSON(cJSON* j){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<SetVoltageThresholdRequest>(voltage, ticket);
return r;
}
SetVoltageThresholdRequest::~SetVoltageThresholdRequest(){
}
