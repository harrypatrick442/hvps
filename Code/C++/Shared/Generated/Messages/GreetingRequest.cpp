#include "./GreetingRequest.hpp"
const char* GreetingRequest::TYPE = "gr";
GreetingRequest::GreetingRequest(
    uint64_t ticket):
        _ticket(ticket){
}
uint64_t GreetingRequest::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GreetingRequest::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GreetingRequest* GreetingRequest::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GreetingRequest(ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GreetingRequest::~GreetingRequest(){
}
