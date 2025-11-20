#include "./NativeDisposeClientToClientChannelMessage.hpp"
const char* NativeDisposeClientToClientChannelMessage::TYPE = "ndctcc";
NativeDisposeClientToClientChannelMessage::NativeDisposeClientToClientChannelMessage(
)
{
}
cJSON* NativeDisposeClientToClientChannelMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativeDisposeClientToClientChannelMessage* NativeDisposeClientToClientChannelMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new NativeDisposeClientToClientChannelMessage();
    cleanupBucket.addDelete(r);
    return r;
}
NativeDisposeClientToClientChannelMessage::~NativeDisposeClientToClientChannelMessage(){
}
