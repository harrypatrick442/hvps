#include "./NativeReadyMessage.hpp"
const char* NativeReadyMessage::TYPE = "nr";
NativeReadyMessage::NativeReadyMessage(
)
{
}
cJSON* NativeReadyMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeReadyMessage* NativeReadyMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new NativeReadyMessage();
    cleanupBucket.addDelete(r);
    return r;
}
NativeReadyMessage::~NativeReadyMessage(){
}
