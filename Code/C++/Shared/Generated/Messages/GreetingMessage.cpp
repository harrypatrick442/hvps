#include "./GreetingMessage.hpp"
const char* GreetingMessage::TYPE = "g";
GreetingMessage::GreetingMessage(
    std::shared_ptr<CoreDumpSummaryMessage>, 
    std::shared_ptr<LastAbortMessage>):
        _coreDumpSummaryMessage(coreDumpSummaryMessage),
        _lastAbortMessage(lastAbortMessage){
}
std::shared_ptr<CoreDumpSummaryMessage> GreetingMessage::getCoreDumpSummaryMessage(){
    return this->_coreDumpSummaryMessage;
}
std::shared_ptr<LastAbortMessage> GreetingMessage::getLastAbortMessage(){
    return this->_lastAbortMessage;
}
cJSON* GreetingMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableObject(j, "cd", this->_coreDumpSummaryMessage->toJSON());
    JHelper::addNullableObject(j, "la", this->_lastAbortMessage->toJSON());
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GreetingMessage> GreetingMessage::fromJSON(cJSON* j){
    bool s = true;
    cJSON* coreDumpSummaryMessageJSON = JHelper::getNullableObject(j, "cd", s);
    std::shared_ptr<CoreDumpSummaryMessage> coreDumpSummaryMessage = coreDumpSummaryMessageJSON==nullptr?nullptr:CoreDumpSummaryMessage::fromJSON(coreDumpSummaryMessageJSON);
    cJSON* lastAbortMessageJSON = JHelper::getNullableObject(j, "la", s);
    std::shared_ptr<LastAbortMessage> lastAbortMessage = lastAbortMessageJSON==nullptr?nullptr:LastAbortMessage::fromJSON(lastAbortMessageJSON);
    auto r = std::make_shared<GreetingMessage>(coreDumpSummaryMessage, lastAbortMessage);
return r;
}
GreetingMessage::~GreetingMessage(){
}
