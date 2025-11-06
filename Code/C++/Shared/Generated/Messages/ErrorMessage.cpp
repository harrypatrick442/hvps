#include "./ErrorMessage.hpp"
const char* ErrorMessage::TYPE = "e";
ErrorMessage::ErrorMessage(
    int32_t errorType, 
    const char* serializedError):
        _errorType(errorType),
        _serializedError(serializedError){
}
int32_t ErrorMessage::getErrorType(){
    return this->_errorType;
}
const char* ErrorMessage::getSerializedError(){
    return this->_serializedError;
}
cJSON* ErrorMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "t", this->_errorType);
    JHelper::addString(j, "s", this->_serializedError);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<ErrorMessage> ErrorMessage::fromJSON(cJSON* j){
    bool s = true;
    int32_t errorType = JHelper::getInt32(j, "t", s);
    const char* serializedError = JHelper::getString(j, "s", s);
    return std::make_shared<ErrorMessage>(errorType, serializedError);
}
ErrorMessage::~ErrorMessage(){
}
