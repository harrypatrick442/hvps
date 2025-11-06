#include "./StopMessage.hpp"
const char* StopMessage::TYPE = "stop";
StopMessage::StopMessage(
)
{
}
cJSON* StopMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<StopMessage> StopMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<StopMessage>();
}
StopMessage::~StopMessage(){
}
