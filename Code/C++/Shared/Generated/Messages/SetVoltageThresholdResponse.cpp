#include "./SetVoltageThresholdResponse.hpp"
const char* SetVoltageThresholdResponse::TYPE = "tkd";
SetVoltageThresholdResponse::SetVoltageThresholdResponse(
    float voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
float SetVoltageThresholdResponse::getVoltage()const noexcept{
    return this->_voltage;
}
uint64_t SetVoltageThresholdResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* SetVoltageThresholdResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addFloat(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
SetVoltageThresholdResponse* SetVoltageThresholdResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    float voltage = JHelper::getFloat(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new SetVoltageThresholdResponse(voltage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
SetVoltageThresholdResponse::~SetVoltageThresholdResponse(){
}
