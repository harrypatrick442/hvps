#include "./GetVoltageResponse.hpp"
const char* GetVoltageResponse::TYPE = "tkd";
GetVoltageResponse::GetVoltageResponse(
    float voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
float GetVoltageResponse::getVoltage()const noexcept{
    return this->_voltage;
}
uint64_t GetVoltageResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetVoltageResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addFloat(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetVoltageResponse* GetVoltageResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    float voltage = JHelper::getFloat(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetVoltageResponse(voltage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetVoltageResponse::~GetVoltageResponse(){
}
