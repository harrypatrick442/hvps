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
std::shared_ptr<NativeCloseFileMessage> NativeCloseFileMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<NativeCloseFileMessage>();
}
NativeCloseFileMessage::~NativeCloseFileMessage(){
}
