#include "./Message.hpp"
const char* Message::TYPE = "m";
Message::Message(
    const char* content):
        _content(content),
        _freeMemoryInDeconstructor(false){
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
r->_freeMemoryInDeconstructor = true;
return r;
}
Message::~Message(){
if(!_freeMemoryInDeconstructor)return;
     if(_content!=nullptr)delete[] _content;
}
