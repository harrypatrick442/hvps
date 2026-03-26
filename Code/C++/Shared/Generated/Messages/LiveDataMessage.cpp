#include "./LiveDataMessage.hpp"
const char* LiveDataMessage::TYPE = "ld";
LiveDataMessage::LiveDataMessage(
    uint8_t firstStageVoltageValueBoundType, 
    float firstStageVoltageVolts, 
    float frequencyHz, 
    uint8_t frequencyHzValueBoundType, 
    float mosfetTemperatureDegreesC, 
    float outputCurrentAmps, 
    uint8_t outputVoltageValueBoundType, 
    float outputVoltageVolts, 
    float peakPrimaryCurrentAmps, 
    uint8_t peakPrimaryCurrentValueBoundType, 
    float primaryPowerWatts, 
    float totalOutputEnergyJouls, 
    float totalPrimaryEnergyJouls):
        _firstStageVoltageValueBoundType(firstStageVoltageValueBoundType),
        _firstStageVoltageVolts(firstStageVoltageVolts),
        _frequencyHz(frequencyHz),
        _frequencyHzValueBoundType(frequencyHzValueBoundType),
        _mosfetTemperatureDegreesC(mosfetTemperatureDegreesC),
        _outputCurrentAmps(outputCurrentAmps),
        _outputVoltageValueBoundType(outputVoltageValueBoundType),
        _outputVoltageVolts(outputVoltageVolts),
        _peakPrimaryCurrentAmps(peakPrimaryCurrentAmps),
        _peakPrimaryCurrentValueBoundType(peakPrimaryCurrentValueBoundType),
        _primaryPowerWatts(primaryPowerWatts),
        _totalOutputEnergyJouls(totalOutputEnergyJouls),
        _totalPrimaryEnergyJouls(totalPrimaryEnergyJouls){
}
uint8_t LiveDataMessage::getFirstStageVoltageValueBoundType()const noexcept{
    return this->_firstStageVoltageValueBoundType;
}
float LiveDataMessage::getFirstStageVoltageVolts()const noexcept{
    return this->_firstStageVoltageVolts;
}
float LiveDataMessage::getFrequencyHz()const noexcept{
    return this->_frequencyHz;
}
uint8_t LiveDataMessage::getFrequencyHzValueBoundType()const noexcept{
    return this->_frequencyHzValueBoundType;
}
float LiveDataMessage::getMosfetTemperatureDegreesC()const noexcept{
    return this->_mosfetTemperatureDegreesC;
}
float LiveDataMessage::getOutputCurrentAmps()const noexcept{
    return this->_outputCurrentAmps;
}
uint8_t LiveDataMessage::getOutputVoltageValueBoundType()const noexcept{
    return this->_outputVoltageValueBoundType;
}
float LiveDataMessage::getOutputVoltageVolts()const noexcept{
    return this->_outputVoltageVolts;
}
float LiveDataMessage::getPeakPrimaryCurrentAmps()const noexcept{
    return this->_peakPrimaryCurrentAmps;
}
uint8_t LiveDataMessage::getPeakPrimaryCurrentValueBoundType()const noexcept{
    return this->_peakPrimaryCurrentValueBoundType;
}
float LiveDataMessage::getPrimaryPowerWatts()const noexcept{
    return this->_primaryPowerWatts;
}
float LiveDataMessage::getTotalOutputEnergyJouls()const noexcept{
    return this->_totalOutputEnergyJouls;
}
float LiveDataMessage::getTotalPrimaryEnergyJouls()const noexcept{
    return this->_totalPrimaryEnergyJouls;
}
cJSON* LiveDataMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addUInt8(j, "f", this->_firstStageVoltageValueBoundType);
    JHelper::addFloat(j, "e", this->_firstStageVoltageVolts);
    JHelper::addFloat(j, "g", this->_frequencyHz);
    JHelper::addUInt8(j, "h", this->_frequencyHzValueBoundType);
    JHelper::addFloat(j, "m", this->_mosfetTemperatureDegreesC);
    JHelper::addFloat(j, "c", this->_outputCurrentAmps);
    JHelper::addUInt8(j, "b", this->_outputVoltageValueBoundType);
    JHelper::addFloat(j, "a", this->_outputVoltageVolts);
    JHelper::addFloat(j, "i", this->_peakPrimaryCurrentAmps);
    JHelper::addUInt8(j, "l", this->_peakPrimaryCurrentValueBoundType);
    JHelper::addFloat(j, "j", this->_primaryPowerWatts);
    JHelper::addFloat(j, "d", this->_totalOutputEnergyJouls);
    JHelper::addFloat(j, "k", this->_totalPrimaryEnergyJouls);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
LiveDataMessage* LiveDataMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    uint8_t firstStageVoltageValueBoundType = JHelper::getUInt8(j, "f", s);
    float firstStageVoltageVolts = JHelper::getFloat(j, "e", s);
    float frequencyHz = JHelper::getFloat(j, "g", s);
    uint8_t frequencyHzValueBoundType = JHelper::getUInt8(j, "h", s);
    float mosfetTemperatureDegreesC = JHelper::getFloat(j, "m", s);
    float outputCurrentAmps = JHelper::getFloat(j, "c", s);
    uint8_t outputVoltageValueBoundType = JHelper::getUInt8(j, "b", s);
    float outputVoltageVolts = JHelper::getFloat(j, "a", s);
    float peakPrimaryCurrentAmps = JHelper::getFloat(j, "i", s);
    uint8_t peakPrimaryCurrentValueBoundType = JHelper::getUInt8(j, "l", s);
    float primaryPowerWatts = JHelper::getFloat(j, "j", s);
    float totalOutputEnergyJouls = JHelper::getFloat(j, "d", s);
    float totalPrimaryEnergyJouls = JHelper::getFloat(j, "k", s);
    auto r = new LiveDataMessage(firstStageVoltageValueBoundType, firstStageVoltageVolts, frequencyHz, frequencyHzValueBoundType, mosfetTemperatureDegreesC, outputCurrentAmps, outputVoltageValueBoundType, outputVoltageVolts, peakPrimaryCurrentAmps, peakPrimaryCurrentValueBoundType, primaryPowerWatts, totalOutputEnergyJouls, totalPrimaryEnergyJouls);
    cleanupBucket.addDelete(r);
    return r;
}
LiveDataMessage::~LiveDataMessage(){
}
