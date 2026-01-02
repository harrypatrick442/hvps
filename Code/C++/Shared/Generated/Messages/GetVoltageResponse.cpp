#include "./GetVoltageResponse.hpp"
const char* GetVoltageResponse::TYPE = "tkd";
GetVoltageResponse::GetVoltageResponse(
    uint16_t rawVoltage, 
    float voltage, 
    uint64_t ticket):
        _rawVoltage(rawVoltage),
        _voltage(voltage),
        _ticket(ticket){
}
uint16_t GetVoltageResponse::getRawVoltage()const noexcept{
    return this->_rawVoltage;
}
float GetVoltageResponse::getVoltage()const noexcept{
    return this->_voltage;
}
uint64_t GetVoltageResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetVoltageResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt16(j, "r", this->_rawVoltage);
    JHelper::addFloat(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetVoltageResponse* GetVoltageResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint16_t rawVoltage = JHelper::getUInt16(j, "r", s);
    float voltage = JHelper::getFloat(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetVoltageResponse(rawVoltage, voltage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetVoltageResponse::~GetVoltageResponse(){
}
