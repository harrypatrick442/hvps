#include "./GreetingMessage.hpp"
const char* GreetingMessage::TYPE = "g";
GreetingMessage::GreetingMessage(
    CoreDumpSummaryMessage* coreDumpSummaryMessage, 
    LastAbortMessage* lastAbortMessage):
        _coreDumpSummaryMessage(coreDumpSummaryMessage),
        _lastAbortMessage(lastAbortMessage){
}
CoreDumpSummaryMessage* GreetingMessage::getCoreDumpSummaryMessage()const noexcept{
    return this->_coreDumpSummaryMessage;
}
LastAbortMessage* GreetingMessage::getLastAbortMessage()const noexcept{
    return this->_lastAbortMessage;
}
cJSON* GreetingMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableObject(j, "cd", this->_coreDumpSummaryMessage==nullptr?nullptr:this->_coreDumpSummaryMessage->toJSON());
    JHelper::addNullableObject(j, "la", this->_lastAbortMessage==nullptr?nullptr:this->_lastAbortMessage->toJSON());
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GreetingMessage* GreetingMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    cJSON* coreDumpSummaryMessageJSON = JHelper::getNullableObject(j, "cd", s);
    CoreDumpSummaryMessage* coreDumpSummaryMessage = coreDumpSummaryMessageJSON==nullptr?nullptr:CoreDumpSummaryMessage::fromJSON(coreDumpSummaryMessageJSON, cleanupBucket);
    cJSON* lastAbortMessageJSON = JHelper::getNullableObject(j, "la", s);
    LastAbortMessage* lastAbortMessage = lastAbortMessageJSON==nullptr?nullptr:LastAbortMessage::fromJSON(lastAbortMessageJSON, cleanupBucket);
    auto r = new GreetingMessage(coreDumpSummaryMessage, lastAbortMessage);
    cleanupBucket.addDelete(r);
    return r;
}
GreetingMessage::~GreetingMessage(){
}
