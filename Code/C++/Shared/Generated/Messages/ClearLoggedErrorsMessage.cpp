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
std::shared_ptr<ClearLoggedErrorsMessage> ClearLoggedErrorsMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<ClearLoggedErrorsMessage>();
return r;
}
ClearLoggedErrorsMessage::~ClearLoggedErrorsMessage(){
}
