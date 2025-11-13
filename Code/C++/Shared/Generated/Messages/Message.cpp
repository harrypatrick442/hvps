#include "./Message.hpp"
const char* Message::TYPE = "m";
Message::Message(
    const char* content):
        _content(content){
}
const char* Message::getContent(){
    return this->_content;
}
cJSON* Message::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "c", this->_content);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<Message> Message::fromJSON(cJSON* j){
    bool s = true;
    const char* content = JHelper::getString(j, "c", s);
    auto r = std::make_shared<Message>(content);
return r;
}
Message::~Message(){
}
