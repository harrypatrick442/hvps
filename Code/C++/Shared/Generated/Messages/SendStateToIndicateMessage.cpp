#include "./SendStateToIndicateMessage.hpp"
const char* SendStateToIndicateMessage::TYPE = "ssti";
SendStateToIndicateMessage::SendStateToIndicateMessage(
)
{
}
cJSON* SendStateToIndicateMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
SendStateToIndicateMessage* SendStateToIndicateMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new SendStateToIndicateMessage();
    cleanupBucket.addDelete(r);
    return r;
}
SendStateToIndicateMessage::~SendStateToIndicateMessage(){
}
