#include "./StartMessage.hpp"
const char* StartMessage::TYPE = "start";
StartMessage::StartMessage(
)
{
}
cJSON* StartMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<StartMessage> StartMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<StartMessage>();
}
StartMessage::~StartMessage(){
}
