#include "./RanSystemChecksMessage.hpp"
const char* RanSystemChecksMessage::TYPE = "cm";
RanSystemChecksMessage::RanSystemChecksMessage(
)
{
}
cJSON* RanSystemChecksMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<RanSystemChecksMessage> RanSystemChecksMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<RanSystemChecksMessage>();
return r;
}
RanSystemChecksMessage::~RanSystemChecksMessage(){
}
