#include "./GreetingResponse.hpp"
const char* GreetingResponse::TYPE = "tkd";
GreetingResponse::GreetingResponse(
    std::shared_ptr<CoreDumpSummaryMessage>, 
    std::shared_ptr<LastAbortMessage>, 
    uint64_t ticket):
        _coreDumpSummaryMessage(coreDumpSummaryMessage),
        _lastAbortMessage(lastAbortMessage),
        _ticket(ticket){
}
std::shared_ptr<CoreDumpSummaryMessage> GreetingResponse::getCoreDumpSummaryMessage(){
    return this->_coreDumpSummaryMessage;
}
std::shared_ptr<LastAbortMessage> GreetingResponse::getLastAbortMessage(){
    return this->_lastAbortMessage;
}
uint64_t GreetingResponse::getTicket(){
    return this->_ticket;
}
cJSON* GreetingResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableObject(j, "cd", this->_coreDumpSummaryMessage->toJSON());
    JHelper::addNullableObject(j, "la", this->_lastAbortMessage->toJSON());
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GreetingResponse> GreetingResponse::fromJSON(cJSON* j){
    bool s = true;
    cJSON* coreDumpSummaryMessageJSON = JHelper::getNullableObject(j, "cd", s);
    std::shared_ptr<CoreDumpSummaryMessage> coreDumpSummaryMessage = coreDumpSummaryMessageJSON==nullptr?nullptr:CoreDumpSummaryMessage::fromJSON(coreDumpSummaryMessageJSON);
    cJSON* lastAbortMessageJSON = JHelper::getNullableObject(j, "la", s);
    std::shared_ptr<LastAbortMessage> lastAbortMessage = lastAbortMessageJSON==nullptr?nullptr:LastAbortMessage::fromJSON(lastAbortMessageJSON);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = std::make_shared<GreetingResponse>(coreDumpSummaryMessage, lastAbortMessage, ticket);
return r;
}
GreetingResponse::~GreetingResponse(){
}
