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
StartMessage* StartMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new StartMessage();
    cleanupBucket.addDelete(r);
    return r;
}
StartMessage::~StartMessage(){
}
