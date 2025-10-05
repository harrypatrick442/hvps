#include "./HVPSShutDownMessage.hpp"
const char* HVPSShutDownMessage::TYPE = "sd";
HVPSShutDownMessage::HVPSShutDownMessage(
)
{
}
cJSON* HVPSShutDownMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSShutDownMessage> HVPSShutDownMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<HVPSShutDownMessage>();
}
HVPSShutDownMessage::~HVPSShutDownMessage(){
}
