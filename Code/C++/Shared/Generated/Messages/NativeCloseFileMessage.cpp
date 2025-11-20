#include "./NativeCloseFileMessage.hpp"
const char* NativeCloseFileMessage::TYPE = "ncf";
NativeCloseFileMessage::NativeCloseFileMessage(
)
{
}
cJSON* NativeCloseFileMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeCloseFileMessage* NativeCloseFileMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new NativeCloseFileMessage();
    cleanupBucket.addDelete(r);
    return r;
}
NativeCloseFileMessage::~NativeCloseFileMessage(){
}
