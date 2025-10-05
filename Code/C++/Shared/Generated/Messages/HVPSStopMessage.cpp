#include "./HVPSStopMessage.hpp"
const char* HVPSStopMessage::TYPE = "stop";
HVPSStopMessage::HVPSStopMessage(
)
{
}
cJSON* HVPSStopMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSStopMessage> HVPSStopMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<HVPSStopMessage>();
}
HVPSStopMessage::~HVPSStopMessage(){
}
