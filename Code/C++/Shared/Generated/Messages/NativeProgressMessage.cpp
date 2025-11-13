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
std::shared_ptr<NativeProgressMessage> NativeProgressMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<NativeProgressMessage>();
return r;
}
NativeProgressMessage::~NativeProgressMessage(){
}
