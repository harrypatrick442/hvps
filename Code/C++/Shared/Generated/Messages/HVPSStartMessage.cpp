#include "./HVPSStartMessage.hpp"
const char* HVPSStartMessage::TYPE = "start";
HVPSStartMessage::HVPSStartMessage(
)
{
}
cJSON* HVPSStartMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSStartMessage> HVPSStartMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<HVPSStartMessage>();
}
HVPSStartMessage::~HVPSStartMessage(){
}
