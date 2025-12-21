#pragma once
#include "Interfaces/ISystemStateIndicator.hpp"
#include "Enums/SystemState.hpp"
#include "HVPSLEDDisplay.hpp"
class SystemStateIndicator final:
	public SingletonBase<SystemStateIndicator>,
	public ISystemStateIndicator
{
    friend class SingletonBase<SystemStateIndicator>;
	public:
		static const char* getTag();
	private:
		HVPSLEDDisplay& _hVPSLEDDisplay;
	public:
		DISALLOW_COPY_MOVE(SystemStateIndicator);
		void indicateState(SystemState systemState) noexcept;
	protected:
		explicit SystemStateIndicator(HVPSLEDDisplay& hVPSLEDDisplay)noexcept;
};