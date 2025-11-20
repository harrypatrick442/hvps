#include "./NativeGotNewTokenMessage.hpp"
const char* NativeGotNewTokenMessage::TYPE = "ngnt";
NativeGotNewTokenMessage::NativeGotNewTokenMessage(
)
{
}
cJSON* NativeGotNewTokenMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeGotNewTokenMessage* NativeGotNewTokenMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new NativeGotNewTokenMessage();
    cleanupBucket.addDelete(r);
    return r;
}
NativeGotNewTokenMessage::~NativeGotNewTokenMessage(){
}
