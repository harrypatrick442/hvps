#include "./VoltageMessage.hpp"
const char* VoltageMessage::TYPE = "vol";
VoltageMessage::VoltageMessage(
    double voltage):
        _voltage(voltage){
}
double VoltageMessage::getVoltage()const noexcept{
    return this->_voltage;
}
cJSON* VoltageMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
VoltageMessage* VoltageMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    auto r = new VoltageMessage(voltage);
    cleanupBucket.addDelete(r);
    return r;
}
VoltageMessage::~VoltageMessage(){
}
