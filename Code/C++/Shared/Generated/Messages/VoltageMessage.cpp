#include "./VoltageMessage.hpp"
const char* VoltageMessage::TYPE = "vol";
VoltageMessage::VoltageMessage(
    uint16_t rawVoltage, 
    float voltage):
        _rawVoltage(rawVoltage),
        _voltage(voltage){
}
uint16_t VoltageMessage::getRawVoltage()const noexcept{
    return this->_rawVoltage;
}
float VoltageMessage::getVoltage()const noexcept{
    return this->_voltage;
}
cJSON* VoltageMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt16(j, "r", this->_rawVoltage);
    JHelper::addFloat(j, "v", this->_voltage);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
VoltageMessage* VoltageMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint16_t rawVoltage = JHelper::getUInt16(j, "r", s);
    float voltage = JHelper::getFloat(j, "v", s);
    auto r = new VoltageMessage(rawVoltage, voltage);
    cleanupBucket.addDelete(r);
    return r;
}
VoltageMessage::~VoltageMessage(){
}
