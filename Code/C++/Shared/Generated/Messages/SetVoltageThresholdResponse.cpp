#include "./SetVoltageThresholdResponse.hpp"
const char* SetVoltageThresholdResponse::TYPE = "tkd";
SetVoltageThresholdResponse::SetVoltageThresholdResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t SetVoltageThresholdResponse::getTicket(){
    return this->_ticket;
}
cJSON* SetVoltageThresholdResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<SetVoltageThresholdResponse> SetVoltageThresholdResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<SetVoltageThresholdResponse>(ticket);
}
SetVoltageThresholdResponse::~SetVoltageThresholdResponse(){
}
