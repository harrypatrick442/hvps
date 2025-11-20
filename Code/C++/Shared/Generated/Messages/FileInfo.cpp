#include "./FileInfo.hpp"
FileInfo::FileInfo(
    const char* name, 
    int64_t size, 
    const char* type):
        _name(name),
        _size(size),
        _type(type){
}
const char* FileInfo::getName()const noexcept{
    return this->_name;
}
int64_t FileInfo::getSize()const noexcept{
    return this->_size;
}
const char* FileInfo::getType()const noexcept{
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
FileInfo* FileInfo::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* name = JHelper::getString(j, "n", s);
    cleanupBucket.addDeleteArray(name);
    int64_t size = JHelper::getInt64(j, "s", s);
    const char* type = JHelper::getString(j, "t", s);
    cleanupBucket.addDeleteArray(type);
    auto r = new FileInfo(name, size, type);
    cleanupBucket.addDelete(r);
    return r;
}
FileInfo::~FileInfo(){
}
