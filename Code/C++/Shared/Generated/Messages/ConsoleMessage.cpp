#include "./ConsoleMessage.hpp"
const char* ConsoleMessage::TYPE = "cmsg";
ConsoleMessage::ConsoleMessage(
    bool isError, 
    const char* message):
        _isError(isError),
        _message(message),
        _freeMemoryInDeconstructor(false){
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
r->_freeMemoryInDeconstructor = true;
return r;
}
ConsoleMessage::~ConsoleMessage(){
if(!_freeMemoryInDeconstructor)return;
     if(_message!=nullptr)delete[] _message;
}
