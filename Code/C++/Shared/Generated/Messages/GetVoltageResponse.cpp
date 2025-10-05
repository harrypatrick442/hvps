#include "./GetVoltageResponse.hpp"
const char* GetVoltageResponse::TYPE = "tkd";
GetVoltageResponse::GetVoltageResponse(
    double voltage, 
    uint64_t ticket):
        _voltage(voltage),
        _ticket(ticket){
}
double GetVoltageResponse::getVoltage(){
    return this->_voltage;
}
uint64_t GetVoltageResponse::getTicket(){
    return this->_ticket;
}
cJSON* GetVoltageResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetVoltageResponse> GetVoltageResponse::fromJSON(cJSON* j){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<GetVoltageResponse>(voltage, ticket);
}
GetVoltageResponse::~GetVoltageResponse(){
}
