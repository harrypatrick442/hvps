#include "./SetForceVoltageThresholdReachedFeedbackRequest.hpp"
const char* SetForceVoltageThresholdReachedFeedbackRequest::TYPE = "sft";
SetForceVoltageThresholdReachedFeedbackRequest::SetForceVoltageThresholdReachedFeedbackRequest(
    bool force, 
    uint64_t ticket):
        _force(force),
        _ticket(ticket){
}
bool SetForceVoltageThresholdReachedFeedbackRequest::getForce(){
    return this->_force;
}
uint64_t SetForceVoltageThresholdReachedFeedbackRequest::getTicket(){
    return this->_ticket;
}
cJSON* SetForceVoltageThresholdReachedFeedbackRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "f", this->_force);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<SetForceVoltageThresholdReachedFeedbackRequest> SetForceVoltageThresholdReachedFeedbackRequest::fromJSON(cJSON* j){
    bool s = true;
    bool force = JHelper::getBool(j, "f", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<SetForceVoltageThresholdReachedFeedbackRequest>(force, ticket);
}
SetForceVoltageThresholdReachedFeedbackRequest::~SetForceVoltageThresholdReachedFeedbackRequest(){
}
