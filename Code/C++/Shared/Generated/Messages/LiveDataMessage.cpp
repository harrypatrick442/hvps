#include "./LiveDataMessage.hpp"
const char* LiveDataMessage::TYPE = "ld";
LiveDataMessage::LiveDataMessage(
    float firstStageVoltage, 
    std::optional<uint32_t> frequency, 
    float outputCurrent, 
    float outputVoltage, 
    float peakPrimaryCurrent, 
    float totalOutputEnergy):
        _firstStageVoltage(firstStageVoltage),
        _frequency(frequency),
        _outputCurrent(outputCurrent),
        _outputVoltage(outputVoltage),
        _peakPrimaryCurrent(peakPrimaryCurrent),
        _totalOutputEnergy(totalOutputEnergy){
}
float LiveDataMessage::getFirstStageVoltage()const noexcept{
    return this->_firstStageVoltage;
}
std::optional<uint32_t> LiveDataMessage::getFrequency()const noexcept{
    return this->_frequency;
}
float LiveDataMessage::getOutputCurrent()const noexcept{
    return this->_outputCurrent;
}
float LiveDataMessage::getOutputVoltage()const noexcept{
    return this->_outputVoltage;
}
float LiveDataMessage::getPeakPrimaryCurrent()const noexcept{
    return this->_peakPrimaryCurrent;
}
float LiveDataMessage::getTotalOutputEnergy()const noexcept{
    return this->_totalOutputEnergy;
}
cJSON* LiveDataMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addFloat(j, "f", this->_firstStageVoltage);
    JHelper::addNullableUInt32(j, "fr", this->_frequency);
    JHelper::addFloat(j, "c", this->_outputCurrent);
    JHelper::addFloat(j, "v", this->_outputVoltage);
    JHelper::addFloat(j, "p", this->_peakPrimaryCurrent);
    JHelper::addFloat(j, "t", this->_totalOutputEnergy);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
LiveDataMessage* LiveDataMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    float firstStageVoltage = JHelper::getFloat(j, "f", s);
    std::optional<uint32_t> frequency = JHelper::getNullableUInt32(j, "fr", s);
    float outputCurrent = JHelper::getFloat(j, "c", s);
    float outputVoltage = JHelper::getFloat(j, "v", s);
    float peakPrimaryCurrent = JHelper::getFloat(j, "p", s);
    float totalOutputEnergy = JHelper::getFloat(j, "t", s);
    auto r = new LiveDataMessage(firstStageVoltage, frequency, outputCurrent, outputVoltage, peakPrimaryCurrent, totalOutputEnergy);
    cleanupBucket.addDelete(r);
    return r;
}
LiveDataMessage::~LiveDataMessage(){
}
