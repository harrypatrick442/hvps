#include "./SetForceVoltageThresholdReachedFeedbackResponse.hpp"
const char* SetForceVoltageThresholdReachedFeedbackResponse::TYPE = "tkd";
SetForceVoltageThresholdReachedFeedbackResponse::SetForceVoltageThresholdReachedFeedbackResponse(
    std::optional<bool> force, 
    uint64_t ticket):
        _force(force),
        _ticket(ticket){
}
std::optional<bool> SetForceVoltageThresholdReachedFeedbackResponse::getForce()const noexcept{
    return this->_force;
}
uint64_t SetForceVoltageThresholdReachedFeedbackResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* SetForceVoltageThresholdReachedFeedbackResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableBool(j, "f", this->_force);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
SetForceVoltageThresholdReachedFeedbackResponse* SetForceVoltageThresholdReachedFeedbackResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    std::optional<bool> force = JHelper::getNullableBool(j, "f", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new SetForceVoltageThresholdReachedFeedbackResponse(force, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
SetForceVoltageThresholdReachedFeedbackResponse::~SetForceVoltageThresholdReachedFeedbackResponse(){
}
