#include "./LiveDataMessage.hpp"
const char* LiveDataMessage::TYPE = "ld";
LiveDataMessage::LiveDataMessage(
    double firstStageVoltage, 
    double outputCurrent, 
    double outputVoltage, 
    double peakPrimaryCurrent, 
    double totalOutputEnergy):
        _firstStageVoltage(firstStageVoltage),
        _outputCurrent(outputCurrent),
        _outputVoltage(outputVoltage),
        _peakPrimaryCurrent(peakPrimaryCurrent),
        _totalOutputEnergy(totalOutputEnergy){
}
double LiveDataMessage::getFirstStageVoltage(){
    return this->_firstStageVoltage;
}
double LiveDataMessage::getOutputCurrent(){
    return this->_outputCurrent;
}
double LiveDataMessage::getOutputVoltage(){
    return this->_outputVoltage;
}
double LiveDataMessage::getPeakPrimaryCurrent(){
    return this->_peakPrimaryCurrent;
}
double LiveDataMessage::getTotalOutputEnergy(){
    return this->_totalOutputEnergy;
}
cJSON* LiveDataMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "f", this->_firstStageVoltage);
    JHelper::addDouble(j, "c", this->_outputCurrent);
    JHelper::addDouble(j, "v", this->_outputVoltage);
    JHelper::addDouble(j, "p", this->_peakPrimaryCurrent);
    JHelper::addDouble(j, "t", this->_totalOutputEnergy);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<LiveDataMessage> LiveDataMessage::fromJSON(cJSON* j){
    bool s = true;
    double firstStageVoltage = JHelper::getDouble(j, "f", s);
    double outputCurrent = JHelper::getDouble(j, "c", s);
    double outputVoltage = JHelper::getDouble(j, "v", s);
    double peakPrimaryCurrent = JHelper::getDouble(j, "p", s);
    double totalOutputEnergy = JHelper::getDouble(j, "t", s);
    auto r = std::make_shared<LiveDataMessage>(firstStageVoltage, outputCurrent, outputVoltage, peakPrimaryCurrent, totalOutputEnergy);
return r;
}
LiveDataMessage::~LiveDataMessage(){
}
