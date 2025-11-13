#include "./NativePlatformMessage.hpp"
const char* NativePlatformMessage::TYPE = "npt";
NativePlatformMessage::NativePlatformMessage(
    int32_t platform):
        _platform(platform){
}
int32_t NativePlatformMessage::getPlatform(){
    return this->_platform;
}
cJSON* NativePlatformMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "p", this->_platform);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativePlatformMessage> NativePlatformMessage::fromJSON(cJSON* j){
    bool s = true;
    int32_t platform = JHelper::getInt32(j, "p", s);
    auto r = std::make_shared<NativePlatformMessage>(platform);
return r;
}
NativePlatformMessage::~NativePlatformMessage(){
}
