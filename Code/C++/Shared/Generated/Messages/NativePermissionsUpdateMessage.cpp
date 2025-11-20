#include "./NativePermissionsUpdateMessage.hpp"
const char* NativePermissionsUpdateMessage::TYPE = "npu";
NativePermissionsUpdateMessage::NativePermissionsUpdateMessage(
    bool hasAllRequired):
        _hasAllRequired(hasAllRequired){
}
bool NativePermissionsUpdateMessage::getHasAllRequired()const noexcept{
    return this->_hasAllRequired;
}
cJSON* NativePermissionsUpdateMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "a", this->_hasAllRequired);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
NativePermissionsUpdateMessage* NativePermissionsUpdateMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    bool hasAllRequired = JHelper::getBool(j, "a", s);
    auto r = new NativePermissionsUpdateMessage(hasAllRequired);
    cleanupBucket.addDelete(r);
    return r;
}
NativePermissionsUpdateMessage::~NativePermissionsUpdateMessage(){
}
