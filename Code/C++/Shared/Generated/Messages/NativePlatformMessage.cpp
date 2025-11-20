#include "./NativePlatformMessage.hpp"
const char* NativePlatformMessage::TYPE = "npt";
NativePlatformMessage::NativePlatformMessage(
    int32_t platform):
        _platform(platform){
}
int32_t NativePlatformMessage::getPlatform()const noexcept{
    return this->_platform;
}
cJSON* NativePlatformMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "p", this->_platform);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativePlatformMessage* NativePlatformMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t platform = JHelper::getInt32(j, "p", s);
    auto r = new NativePlatformMessage(platform);
    cleanupBucket.addDelete(r);
    return r;
}
NativePlatformMessage::~NativePlatformMessage(){
}
