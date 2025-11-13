#include "./SetForceVoltageThresholdReachedFeedbackResponse.hpp"
const char* SetForceVoltageThresholdReachedFeedbackResponse::TYPE = "tkd";
SetForceVoltageThresholdReachedFeedbackResponse::SetForceVoltageThresholdReachedFeedbackResponse(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t SetForceVoltageThresholdReachedFeedbackResponse::getTicket(){
    return this->_ticket;
}
cJSON* SetForceVoltageThresholdReachedFeedbackResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<SetForceVoltageThresholdReachedFeedbackResponse> SetForceVoltageThresholdReachedFeedbackResponse::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<SetForceVoltageThresholdReachedFeedbackResponse>(ticket);
return r;
}
SetForceVoltageThresholdReachedFeedbackResponse::~SetForceVoltageThresholdReachedFeedbackResponse(){
}
