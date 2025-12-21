#include "LEDStrip.hpp"
#include "System/SafeAbort.hpp"
#include "Graphics/PixelHelper.hpp"
LEDStrip::LEDStrip(
	int dOutPin, 
	size_t length, 
	led_color_component_format_t colourComponentFormat,
	led_model_t ledModel,
	bool invertOut,
	bool isRGBW
)
	:
	_length(length),
	_strip()
{
	//NOT TO GO HERE. MOVE THIS TO SHARED BUFFER. ALSO DONT FORGET GET RID OF OLD PIXEL STRUCT
	
    led_strip_config_t strip_config = {
        .strip_gpio_num = dOutPin,
        .max_leds = length,
        .led_model = ledModel,
        .color_component_format = colourComponentFormat,
    };
    strip_config.flags.invert_out = invertOut;

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10MHz = 0.1us resolution
        .mem_block_symbols = length * 24
    };
    rmt_config.flags.with_dma = false;
	
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &_strip));
    led_strip_clear(_strip); // Clear all LEDs to off
}
LEDStrip::~LEDStrip(){
	//DisplayBuffer will take care of freeing its single buffer.
    if (_strip != nullptr) {
        led_strip_del(_strip);
        _strip = nullptr;
    }
}
size_t LEDStrip::getLength(){
	return _length;
}
void LEDStrip::initialize(DisplayBuffer* displayBuffer){
	_pixels = displayBuffer->takePixels(_length);
}
void LEDStrip::refresh(){
	if(_pixels==nullptr){
		SAFE_ABORT("Not initialized");
	}
	uint8_t r = 0, g = 0, b = 0;
	for(int i=0; i<_length; i++){
		uint32_t pixel = _pixels[i];
		PixelHelper::uInt32ToRGB(pixel, r, g, b);
		led_strip_set_pixel(_strip, i, r, g, b);
	}
	led_strip_refresh(_strip);
}
void LEDStrip::getPixels(size_t& length, volatile uint32_t** pixels){
	if(length>_length){
		SAFE_ABORT("Tried to take %zu pixels when only has %zu available", length, _length);
	}
	for(int i=0; i<length; i++){
		pixels[i]=_pixels+i;
	}
}