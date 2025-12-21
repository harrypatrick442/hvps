#pragma once
#include "led_strip.h"
#include "Graphics/Interfaces/IDisplayElement.hpp"
#include "Graphics/DisplayBuffer.hpp"
#include <stdint.h>
#include <cstddef>
class LEDStrip: public IDisplayElement{
	private:
		size_t _length;
		volatile uint32_t* _pixels;
		led_strip_handle_t _strip;
	public:
		LEDStrip(
			int dOutPin, 
			size_t length, 
			led_color_component_format_t colourComponentFormat = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
			led_model_t ledModel = LED_MODEL_WS2812,
			bool invertOut = false,
			bool isRGBW = false
		);
		~LEDStrip();
		size_t getLength();
		void initialize(DisplayBuffer* displayBuffer);
		void refresh();
		void getPixels(size_t& length, volatile uint32_t** pixels);
};