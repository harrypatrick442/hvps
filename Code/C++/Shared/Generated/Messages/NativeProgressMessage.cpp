#include "./NativeProgressMessage.hpp"
NativeProgressMessage::NativeProgressMessage(
)
{
}
cJSON* NativeProgressMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeProgressMessage* NativeProgressMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new NativeProgressMessage();
    cleanupBucket.addDelete(r);
    return r;
}
NativeProgressMessage::~NativeProgressMessage(){
}
