#include "./HVPSRunSystemChecksOnlyMessage.hpp"
const char* HVPSRunSystemChecksOnlyMessage::TYPE = "rsco";
HVPSRunSystemChecksOnlyMessage::HVPSRunSystemChecksOnlyMessage(
)
{
}
cJSON* HVPSRunSystemChecksOnlyMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSRunSystemChecksOnlyMessage> HVPSRunSystemChecksOnlyMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<HVPSRunSystemChecksOnlyMessage>();
}
HVPSRunSystemChecksOnlyMessage::~HVPSRunSystemChecksOnlyMessage(){
}
