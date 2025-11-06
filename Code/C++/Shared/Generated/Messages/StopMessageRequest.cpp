#include "./StopMessageRequest.hpp"
const char* StopMessageRequest::TYPE = "stop";
StopMessageRequest::StopMessageRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t StopMessageRequest::getTicket(){
    return this->_ticket;
}
cJSON* StopMessageRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<StopMessageRequest> StopMessageRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<StopMessageRequest>(ticket);
}
StopMessageRequest::~StopMessageRequest(){
}
