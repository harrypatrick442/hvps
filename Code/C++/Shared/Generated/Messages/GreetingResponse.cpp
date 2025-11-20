#include "./GreetingResponse.hpp"
const char* GreetingResponse::TYPE = "tkd";
GreetingResponse::GreetingResponse(
    CoreDumpSummaryMessage* coreDumpSummaryMessage, 
    LastAbortMessage* lastAbortMessage, 
    uint64_t ticket):
        _coreDumpSummaryMessage(coreDumpSummaryMessage),
        _lastAbortMessage(lastAbortMessage),
        _ticket(ticket){
}
CoreDumpSummaryMessage* GreetingResponse::getCoreDumpSummaryMessage()const noexcept{
    return this->_coreDumpSummaryMessage;
}
LastAbortMessage* GreetingResponse::getLastAbortMessage()const noexcept{
    return this->_lastAbortMessage;
}
uint64_t GreetingResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GreetingResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableObject(j, "cd", this->_coreDumpSummaryMessage==nullptr?nullptr:this->_coreDumpSummaryMessage->toJSON());
    JHelper::addNullableObject(j, "la", this->_lastAbortMessage==nullptr?nullptr:this->_lastAbortMessage->toJSON());
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GreetingResponse* GreetingResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    cJSON* coreDumpSummaryMessageJSON = JHelper::getNullableObject(j, "cd", s);
    CoreDumpSummaryMessage* coreDumpSummaryMessage = coreDumpSummaryMessageJSON==nullptr?nullptr:CoreDumpSummaryMessage::fromJSON(coreDumpSummaryMessageJSON, cleanupBucket);
    cJSON* lastAbortMessageJSON = JHelper::getNullableObject(j, "la", s);
    LastAbortMessage* lastAbortMessage = lastAbortMessageJSON==nullptr?nullptr:LastAbortMessage::fromJSON(lastAbortMessageJSON, cleanupBucket);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GreetingResponse(coreDumpSummaryMessage, lastAbortMessage, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GreetingResponse::~GreetingResponse(){
}
