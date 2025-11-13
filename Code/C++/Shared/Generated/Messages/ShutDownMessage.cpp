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
    auto r = std::make_shared<ShutDownMessage>();
return r;
}
ShutDownMessage::~ShutDownMessage(){
}
