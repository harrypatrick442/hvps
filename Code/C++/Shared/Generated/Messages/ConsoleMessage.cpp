#include "./ConsoleMessage.hpp"
const char* ConsoleMessage::TYPE = "cmsg";
ConsoleMessage::ConsoleMessage(
    bool isError, 
    const char* message):
        _isError(isError),
        _message(message){
}
bool ConsoleMessage::getIsError(){
    return this->_isError;
}
const char* ConsoleMessage::getMessage(){
    return this->_message;
}
cJSON* ConsoleMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addBool(j, "e", this->_isError);
    JHelper::addString(j, "m", this->_message);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<ConsoleMessage> ConsoleMessage::fromJSON(cJSON* j){
    bool s = true;
    bool isError = JHelper::getBool(j, "e", s);
    const char* message = JHelper::getString(j, "m", s);
    auto r = std::make_shared<ConsoleMessage>(isError, message);
return r;
}
ConsoleMessage::~ConsoleMessage(){
}
