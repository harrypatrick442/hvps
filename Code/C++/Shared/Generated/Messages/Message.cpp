#include "./Message.hpp"
const char* Message::TYPE = "m";
Message::Message(
    const char* content):
        _content(content){
}
const char* Message::getContent()const noexcept{
    return this->_content;
}
cJSON* Message::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "c", this->_content);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
Message* Message::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* content = JHelper::getString(j, "c", s);
    cleanupBucket.addDeleteArray(content);
    auto r = new Message(content);
    cleanupBucket.addDelete(r);
    return r;
}
Message::~Message(){
}
