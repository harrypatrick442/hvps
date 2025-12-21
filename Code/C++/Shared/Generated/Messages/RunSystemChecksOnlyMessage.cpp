#include "./RunSystemChecksOnlyMessage.hpp"
const char* RunSystemChecksOnlyMessage::TYPE = "rsco";
RunSystemChecksOnlyMessage::RunSystemChecksOnlyMessage(
)
{
}
cJSON* RunSystemChecksOnlyMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
RunSystemChecksOnlyMessage* RunSystemChecksOnlyMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new RunSystemChecksOnlyMessage();
    cleanupBucket.addDelete(r);
    return r;
}
RunSystemChecksOnlyMessage::~RunSystemChecksOnlyMessage(){
}
