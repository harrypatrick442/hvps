#pragma once
#include "Core/Event.hpp"
#include "Core/SingletonBase.hpp"
#include "../Generated/HVPSConfiguration.hpp"
#include "Generated/HVPS_FPGAInterface.hpp"
class ThresholdController final : public SingletonBase<ThresholdController>{
private:
	const HVPSConfiguration& _hvpsConfiguration1;
	const HVPSConfiguration& _hvpsConfiguration2;
	HVPS_FPGAInterface& _fpgaInterface;
	EventConnection _eventConnectionFPGAOutputsUpdated;
	float _desiredOutputVoltage;

public:
	static const char* getTag();
	void setDesiredOutputVoltage(float value);
private:
    friend class SingletonBase<ThresholdController>;
	
	ThresholdController(
		const HVPSConfiguration& hvpsConfiguration1,
		const HVPSConfiguration& hvpsConfiguration2,
		HVPS_FPGAInterface& fpgaInterface
	)noexcept;
	void clampDesiredOutputVoltage(float& value);
};