#include "./ShutDownMessage.hpp"
const char* ShutDownMessage::TYPE = "sd";
ShutDownMessage::ShutDownMessage(
)
{
}
cJSON* ShutDownMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<ShutDownMessage> ShutDownMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<ShutDownMessage>();
}
ShutDownMessage::~ShutDownMessage(){
}
