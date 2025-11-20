#include "./TestMessage.hpp"
const char* TestMessage::TYPE = "test";
TestMessage::TestMessage(
)
{
}
cJSON* TestMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
TestMessage* TestMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new TestMessage();
    cleanupBucket.addDelete(r);
    return r;
}
TestMessage::~TestMessage(){
}
