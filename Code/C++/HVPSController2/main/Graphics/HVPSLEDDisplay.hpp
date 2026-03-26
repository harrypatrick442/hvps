#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/HVPSConfiguration.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
#include "Graphics/LEDStrip.hpp"
#include "Graphics/DisplayBuffer.hpp"
class HVPSLEDDisplay:public SingletonBase<HVPSLEDDisplay>{
    friend class SingletonBase<HVPSLEDDisplay>;
public:
	static const char* getTag();
private:
	static size_t LED_STRIP_LENGTH;
	static uint32_t OFF_COLOUR;
	const HVPSConfiguration& _config;
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
	explicit HVPSLEDDisplay(const HVPSConfiguration& config)noexcept;
	~HVPSLEDDisplay();
private:
	void timerCallback();
	void show(uint32_t colour, uint32_t flashDelayMilliseconds);
	void flashColour(uint32_t colour, uint32_t flashDelayMilliseconds);
	void staticColour(uint32_t colour);
	void displayColour(uint32_t colour);
};