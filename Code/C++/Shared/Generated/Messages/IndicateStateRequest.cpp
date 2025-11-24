#include "./IndicateStateRequest.hpp"
const char* IndicateStateRequest::TYPE = "isr";
IndicateStateRequest::IndicateStateRequest(
    int32_t systemState, 
    uint64_t ticket):
        _systemState(systemState),
        _ticket(ticket){
}
int32_t IndicateStateRequest::getSystemState()const noexcept{
    return this->_systemState;
}
uint64_t IndicateStateRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* IndicateStateRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "s", this->_systemState);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
IndicateStateRequest* IndicateStateRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t systemState = JHelper::getInt32(j, "s", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new IndicateStateRequest(systemState, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
IndicateStateRequest::~IndicateStateRequest(){
}
