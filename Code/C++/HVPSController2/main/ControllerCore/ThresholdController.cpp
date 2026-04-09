#include "ThresholdController.hpp"
#include "Macros/GetFileName.hpp"
#include "Storage/Flash.hpp"
const char* FLASH_NAMESPACE = "ThresholdController";
const char* DESIRED_OUTPUT_VOLTAGE = "DOVoltage";
const char* ThresholdController::getTag() {return GET_FILE_NAME;}
ThresholdController::ThresholdController(
	const HVPSConfiguration& hvpsConfiguration1,
	const HVPSConfiguration& hvpsConfiguration2,
	HVPS_FPGAInterface& fpgaInterface) noexcept:
	_hvpsConfiguration1(hvpsConfiguration1),
	_hvpsConfiguration2(hvpsConfiguration2),
	_fpgaInterface(fpgaInterface),
	_desiredOutputVoltage(0){
	/*_eventConnectionFPGAOutputsUpdated = _fpgaInterface.outputsUpdated.addHandler([&](){
		handleOutputsUpdated();
	});*/
	float currentValue;
	if(!Flash::getFloat(FLASH_NAMESPACE, DESIRED_OUTPUT_VOLTAGE, currentValue)){
		currentValue = _hvpsConfiguration1.defaultOutputVoltageVolts;
	}
	clampDesiredOutputVoltage(currentValue);
	_desiredOutputVoltage = currentValue;
	_fpgaInterface.setDesiredOutputVoltage(currentValue);
}
void ThresholdController::setDesiredOutputVoltage(float value){
	clampDesiredOutputVoltage(value);
	_fpgaInterface.setDesiredOutputVoltage(value);
	Flash::setFloat(FLASH_NAMESPACE, DESIRED_OUTPUT_VOLTAGE, value);
	_desiredOutputVoltage = value;
}
void ThresholdController::clampDesiredOutputVoltage(float& value){
	if((value>_hvpsConfiguration1.maxOutputVoltageThresholdVolts)||(value>_hvpsConfiguration2.maxOutputVoltageThresholdVolts)){
		value = _hvpsConfiguration1.maxOutputVoltageThresholdVolts;
	}
}
/*
void ThresholdController::handleOutputsUpdated(){
	
}*/
