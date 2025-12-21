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
StopMessage* StopMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new StopMessage();
    cleanupBucket.addDelete(r);
    return r;
}
StopMessage::~StopMessage(){
}
