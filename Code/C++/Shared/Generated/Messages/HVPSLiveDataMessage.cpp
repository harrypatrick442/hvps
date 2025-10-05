#include "./HVPSLiveDataMessage.hpp"
const char* HVPSLiveDataMessage::TYPE = "ld";
HVPSLiveDataMessage::HVPSLiveDataMessage(
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
double HVPSLiveDataMessage::getFirstStageVoltage(){
    return this->_firstStageVoltage;
}
double HVPSLiveDataMessage::getOutputCurrent(){
    return this->_outputCurrent;
}
double HVPSLiveDataMessage::getOutputVoltage(){
    return this->_outputVoltage;
}
double HVPSLiveDataMessage::getPeakPrimaryCurrent(){
    return this->_peakPrimaryCurrent;
}
double HVPSLiveDataMessage::getTotalOutputEnergy(){
    return this->_totalOutputEnergy;
}
cJSON* HVPSLiveDataMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addDouble(j, "fsv", this->_firstStageVoltage);
    JHelper::addDouble(j, "oc", this->_outputCurrent);
    JHelper::addDouble(j, "ov", this->_outputVoltage);
    JHelper::addDouble(j, "pc", this->_peakPrimaryCurrent);
    JHelper::addDouble(j, "toe", this->_totalOutputEnergy);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<HVPSLiveDataMessage> HVPSLiveDataMessage::fromJSON(cJSON* j){
    bool s = true;
    double firstStageVoltage = JHelper::getDouble(j, "fsv", s);
    double outputCurrent = JHelper::getDouble(j, "oc", s);
    double outputVoltage = JHelper::getDouble(j, "ov", s);
    double peakPrimaryCurrent = JHelper::getDouble(j, "pc", s);
    double totalOutputEnergy = JHelper::getDouble(j, "toe", s);
    return std::make_shared<HVPSLiveDataMessage>(firstStageVoltage, outputCurrent, outputVoltage, peakPrimaryCurrent, totalOutputEnergy);
}
HVPSLiveDataMessage::~HVPSLiveDataMessage(){
}
