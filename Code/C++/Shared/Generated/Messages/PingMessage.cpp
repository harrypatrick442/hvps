#include "./PingMessage.hpp"
const char* PingMessage::TYPE = "p";
PingMessage::PingMessage(
)
{
}
cJSON* PingMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
PingMessage* PingMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new PingMessage();
    cleanupBucket.addDelete(r);
    return r;
}
PingMessage::~PingMessage(){
}
