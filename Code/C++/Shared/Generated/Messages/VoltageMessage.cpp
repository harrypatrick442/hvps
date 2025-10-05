#include "./VoltageMessage.hpp"
const char* VoltageMessage::TYPE = "vol";
VoltageMessage::VoltageMessage(
    double voltage):
        _voltage(voltage){
}
double VoltageMessage::getVoltage(){
    return this->_voltage;
}
cJSON* VoltageMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "v", this->_voltage);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<VoltageMessage> VoltageMessage::fromJSON(cJSON* j){
    bool s = true;
    double voltage = JHelper::getDouble(j, "v", s);
    return std::make_shared<VoltageMessage>(voltage);
}
VoltageMessage::~VoltageMessage(){
}
