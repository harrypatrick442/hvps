#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/Peripheral1Config.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Graphics/LEDStrip.hpp"
#include "Graphics/DisplayBuffer.hpp"
class HVPSLEDDisplay:public SingletonBase<HVPSLEDDisplay>{
    friend class SingletonBase<HVPSLEDDisplay>;
public:
	static inline constexpr const char* TAG = "HVPSLEDDisplay";
private:
	static size_t LED_STRIP_LENGTH;
	static uint32_t OFF_COLOUR;
	const Configuration& _config;
	volatile uint32_t**_pixels;
	uint32_t _currentColour;
	bool _flashing;
	bool _flashingIsOn;
	std::mutex _mutex;
	Timer* _timerFlash;
	LEDStrip* _ledStrip;
	DisplayBuffer* _displayBuffer;
public:
	DISALLOW_COPY_MOVE(HVPSLEDDisplay);
	void indicateState(SystemState systemState);
protected:
	explicit HVPSLEDDisplay(const Configuration& config)noexcept;
	~HVPSLEDDisplay();
private:
	void timerCallback();
	void flashColour(uint32_t colour);
	void staticColour(uint32_t colour);
	void displayColour(uint32_t colour);
};