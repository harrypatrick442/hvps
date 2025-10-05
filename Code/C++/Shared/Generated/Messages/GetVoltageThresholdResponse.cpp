#include "./GetVoltageThresholdResponse.hpp"
const char* GetVoltageThresholdResponse::TYPE = "tkd";
GetVoltageThresholdResponse::GetVoltageThresholdResponse(
    double voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
double GetVoltageThresholdResponse::getVoltage(){
    return this->_voltage;
}
uint64_t GetVoltageThresholdResponse::getTicket(){
    return this->_ticket;
}
cJSON* GetVoltageThresholdResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetVoltageThresholdResponse> GetVoltageThresholdResponse::fromJSON(cJSON* j){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<GetVoltageThresholdResponse>(voltage, ticket);
}
GetVoltageThresholdResponse::~GetVoltageThresholdResponse(){
}
