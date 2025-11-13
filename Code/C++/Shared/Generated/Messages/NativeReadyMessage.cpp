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
std::shared_ptr<NativeReadyMessage> NativeReadyMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<NativeReadyMessage>();
return r;
}
NativeReadyMessage::~NativeReadyMessage(){
}
