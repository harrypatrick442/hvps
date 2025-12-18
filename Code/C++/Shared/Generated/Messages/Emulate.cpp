#include "./Emulate.hpp"
const char* Emulate::TYPE = "em";
Emulate::Emulate(
)
{
}
cJSON* Emulate::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
Emulate* Emulate::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new Emulate();
    cleanupBucket.addDelete(r);
    return r;
}
Emulate::~Emulate(){
}
