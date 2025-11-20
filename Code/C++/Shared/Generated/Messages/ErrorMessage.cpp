#include "./ErrorMessage.hpp"
const char* ErrorMessage::TYPE = "e";
ErrorMessage::ErrorMessage(
    int32_t errorType, 
    const char* serializedError):
        _errorType(errorType),
        _serializedError(serializedError){
}
int32_t ErrorMessage::getErrorType()const noexcept{
    return this->_errorType;
}
const char* ErrorMessage::getSerializedError()const noexcept{
    return this->_serializedError;
}
cJSON* ErrorMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addInt32(j, "t", this->_errorType);
    JHelper::addString(j, "s", this->_serializedError);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
ErrorMessage* ErrorMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    int32_t errorType = JHelper::getInt32(j, "t", s);
    const char* serializedError = JHelper::getString(j, "s", s);
    cleanupBucket.addDeleteArray(serializedError);
    auto r = new ErrorMessage(errorType, serializedError);
    cleanupBucket.addDelete(r);
    return r;
}
ErrorMessage::~ErrorMessage(){
}
