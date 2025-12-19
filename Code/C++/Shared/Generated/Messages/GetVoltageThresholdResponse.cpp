#include "./GetVoltageThresholdResponse.hpp"
const char* GetVoltageThresholdResponse::TYPE = "tkd";
GetVoltageThresholdResponse::GetVoltageThresholdResponse(
    float voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
float GetVoltageThresholdResponse::getVoltage()const noexcept{
    return this->_voltage;
}
uint64_t GetVoltageThresholdResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetVoltageThresholdResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addFloat(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetVoltageThresholdResponse* GetVoltageThresholdResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    float voltage = JHelper::getFloat(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetVoltageThresholdResponse(voltage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetVoltageThresholdResponse::~GetVoltageThresholdResponse(){
}
