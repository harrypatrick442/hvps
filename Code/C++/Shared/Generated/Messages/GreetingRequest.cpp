#include "./GreetingRequest.hpp"
const char* GreetingRequest::TYPE = "gr";
GreetingRequest::GreetingRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GreetingRequest::getTicket(){
    return this->_ticket;
}
cJSON* GreetingRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GreetingRequest> GreetingRequest::fromJSON(cJSON* j){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<GreetingRequest>(ticket);
return r;
}
GreetingRequest::~GreetingRequest(){
}
