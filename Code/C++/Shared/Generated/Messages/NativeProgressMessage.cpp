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
    return std::make_shared<NativeProgressMessage>();
}
NativeProgressMessage::~NativeProgressMessage(){
}
