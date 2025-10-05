#include "./NativePermissionsUpdateMessage.hpp"
const char* NativePermissionsUpdateMessage::TYPE = "npu";
NativePermissionsUpdateMessage::NativePermissionsUpdateMessage(
    bool hasAllRequired):
        _hasAllRequired(hasAllRequired){
}
bool NativePermissionsUpdateMessage::getHasAllRequired(){
    return this->_hasAllRequired;
}
cJSON* NativePermissionsUpdateMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "a", this->_hasAllRequired);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<NativePermissionsUpdateMessage> NativePermissionsUpdateMessage::fromJSON(cJSON* j){
    bool s = true;
    bool hasAllRequired = JHelper::getBool(j, "a", s);
    return std::make_shared<NativePermissionsUpdateMessage>(hasAllRequired);
}
NativePermissionsUpdateMessage::~NativePermissionsUpdateMessage(){
}
