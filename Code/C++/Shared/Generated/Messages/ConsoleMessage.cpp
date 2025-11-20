#include "./ConsoleMessage.hpp"
const char* ConsoleMessage::TYPE = "cmsg";
ConsoleMessage::ConsoleMessage(
    bool isError, 
    const char* message):
        _isError(isError),
        _message(message){
}
bool ConsoleMessage::getIsError()const noexcept{
    return this->_isError;
}
const char* ConsoleMessage::getMessage()const noexcept{
    return this->_message;
}
cJSON* ConsoleMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "e", this->_isError);
    JHelper::addString(j, "m", this->_message);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
ConsoleMessage* ConsoleMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    bool isError = JHelper::getBool(j, "e", s);
    const char* message = JHelper::getString(j, "m", s);
    cleanupBucket.addDeleteArray(message);
    auto r = new ConsoleMessage(isError, message);
    cleanupBucket.addDelete(r);
    return r;
}
ConsoleMessage::~ConsoleMessage(){
}
