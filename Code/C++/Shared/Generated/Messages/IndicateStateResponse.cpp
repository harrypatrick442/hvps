#include "./IndicateStateResponse.hpp"
const char* IndicateStateResponse::TYPE = "tkd";
IndicateStateResponse::IndicateStateResponse(
    bool success, 
    uint64_t ticket):
        _success(success),
        _ticket(ticket){
}
bool IndicateStateResponse::getSuccess()const noexcept{
    return this->_success;
}
uint64_t IndicateStateResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* IndicateStateResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "ss", this->_success);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
IndicateStateResponse* IndicateStateResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    bool success = JHelper::getBool(j, "ss", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new IndicateStateResponse(success, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
IndicateStateResponse::~IndicateStateResponse(){
}
