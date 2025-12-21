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
RanSystemChecksMessage* RanSystemChecksMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new RanSystemChecksMessage();
    cleanupBucket.addDelete(r);
    return r;
}
RanSystemChecksMessage::~RanSystemChecksMessage(){
}
