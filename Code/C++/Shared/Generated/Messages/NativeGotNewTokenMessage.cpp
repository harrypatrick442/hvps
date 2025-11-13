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
std::shared_ptr<NativeGotNewTokenMessage> NativeGotNewTokenMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<NativeGotNewTokenMessage>();
return r;
}
NativeGotNewTokenMessage::~NativeGotNewTokenMessage(){
}
