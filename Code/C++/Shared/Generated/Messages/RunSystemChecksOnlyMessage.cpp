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
std::shared_ptr<RunSystemChecksOnlyMessage> RunSystemChecksOnlyMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<RunSystemChecksOnlyMessage>();
return r;
}
RunSystemChecksOnlyMessage::~RunSystemChecksOnlyMessage(){
}
