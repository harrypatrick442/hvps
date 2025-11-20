#include "./ShutDownMessage.hpp"
const char* ShutDownMessage::TYPE = "sd";
ShutDownMessage::ShutDownMessage(
)
{
}
cJSON* ShutDownMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
ShutDownMessage* ShutDownMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new ShutDownMessage();
    cleanupBucket.addDelete(r);
    return r;
}
ShutDownMessage::~ShutDownMessage(){
}
