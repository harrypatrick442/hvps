#include "./PingMessage.hpp"
const char* PingMessage::TYPE = "p";
PingMessage::PingMessage(
)
{
}
cJSON* PingMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<PingMessage> PingMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<PingMessage>();
return r;
}
PingMessage::~PingMessage(){
}
