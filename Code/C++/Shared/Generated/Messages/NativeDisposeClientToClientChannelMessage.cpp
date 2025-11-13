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
std::shared_ptr<NativeDisposeClientToClientChannelMessage> NativeDisposeClientToClientChannelMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<NativeDisposeClientToClientChannelMessage>();
return r;
}
NativeDisposeClientToClientChannelMessage::~NativeDisposeClientToClientChannelMessage(){
}
