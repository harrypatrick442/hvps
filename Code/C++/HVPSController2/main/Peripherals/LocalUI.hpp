#pragma once
#include "Enums/SystemState.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Event.hpp"
#include "ControllerCore/HighSpeedCore.hpp"
#include "Graphics/HVPSLEDDisplay.hpp"
class LocalUI final : public SingletonBase<LocalUI>{
private:
	HighSpeedCore& _highSpeedCore;
	HVPSLEDDisplay& _hVPSLEDDisplay;
	EventConnection _eventConnectionOnSystemStateChanged;
	
public:

public:
	static const char* getTag();
	void handleSystemStateChanged(SystemState systemState);
private:
    friend class SingletonBase<LocalUI>;
	
	LocalUI(
		HighSpeedCore& highSpeedCore,
		HVPSLEDDisplay& hVPSLEDDisplay
	)noexcept;
	
};
