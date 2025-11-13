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
std::shared_ptr<TestMessage> TestMessage::fromJSON(cJSON* j){
    bool s = true;
    auto r = std::make_shared<TestMessage>();
return r;
}
TestMessage::~TestMessage(){
}
