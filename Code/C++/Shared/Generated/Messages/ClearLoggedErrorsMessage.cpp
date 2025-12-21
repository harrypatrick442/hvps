#include "./ClearLoggedErrorsMessage.hpp"
const char* ClearLoggedErrorsMessage::TYPE = "cle";
ClearLoggedErrorsMessage::ClearLoggedErrorsMessage(
)
{
}
cJSON* ClearLoggedErrorsMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
ClearLoggedErrorsMessage* ClearLoggedErrorsMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new ClearLoggedErrorsMessage();
    cleanupBucket.addDelete(r);
    return r;
}
ClearLoggedErrorsMessage::~ClearLoggedErrorsMessage(){
}
