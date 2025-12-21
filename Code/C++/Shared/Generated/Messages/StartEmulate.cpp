#include "./StartEmulate.hpp"
const char* StartEmulate::TYPE = "stae";
StartEmulate::StartEmulate(
)
{
}
cJSON* StartEmulate::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
StartEmulate* StartEmulate::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    auto r = new StartEmulate();
    cleanupBucket.addDelete(r);
    return r;
}
StartEmulate::~StartEmulate(){
}
