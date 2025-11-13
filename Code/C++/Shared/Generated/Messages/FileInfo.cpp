#include "./FileInfo.hpp"
FileInfo::FileInfo(
    const char* name, 
    int64_t size, 
    const char* type):
        _name(name),
        _size(size),
        _type(type){
}
const char* FileInfo::getName(){
    return this->_name;
}
int64_t FileInfo::getSize(){
    return this->_size;
}
const char* FileInfo::getType(){
    return this->_type;
}
cJSON* FileInfo::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "n", this->_name);
    JHelper::addInt64(j, "s", this->_size);
    JHelper::addString(j, "t", this->_type);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<FileInfo> FileInfo::fromJSON(cJSON* j){
    bool s = true;
    const char* name = JHelper::getString(j, "n", s);
    int64_t size = JHelper::getInt64(j, "s", s);
    const char* type = JHelper::getString(j, "t", s);
    auto r = std::make_shared<FileInfo>(name, size, type);
return r;
}
FileInfo::~FileInfo(){
}
