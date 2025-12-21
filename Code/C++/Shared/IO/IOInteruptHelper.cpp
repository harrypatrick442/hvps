#include "IOInteruptHelper.hpp"
#include "esp_intr_alloc.h"
#include "Logging/Log.hpp"

esp_err_t IOInteruptHelper::setupPinEdgeInterupt(
    int pin,
    gpio_isr_t staticHandlerMethod,
    void* arg,
    bool risingEdge,
    bool fallingEdge,
    bool pullUpEnabled,
    bool pullDownEnabled
) {
    // 1. Configure pin
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << pin);
    cfg.mode = GPIO_MODE_INPUT;

    if (risingEdge && fallingEdge)
        cfg.intr_type = GPIO_INTR_ANYEDGE;
    else if (risingEdge)
        cfg.intr_type = GPIO_INTR_POSEDGE;
    else if (fallingEdge)
        cfg.intr_type = GPIO_INTR_NEGEDGE;
    else
        cfg.intr_type = GPIO_INTR_DISABLE;

    cfg.pull_up_en = pullUpEnabled ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = pullDownEnabled ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;

    gpio_config(&cfg);

	esp_err_t err = installISRHandlerIfNotAlready();
	if(err!=ESP_OK){
		return err;
	}
	
    return gpio_isr_handler_add((gpio_num_t)pin, staticHandlerMethod, arg);
}
esp_err_t IOInteruptHelper::removeHandlerAndDisableEdgeInterupt(int pin){
	esp_err_t err1 = gpio_isr_handler_remove((gpio_num_t)pin);
	esp_err_t err2 = gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_DISABLE);
	if(err1!=ESP_OK){
		return err1;
	}
	return err2;
}
esp_err_t IOInteruptHelper::installISRHandlerIfNotAlready(){
    static bool isr_service_installed = false;
    if (isr_service_installed)
    {
		LOG_INFO("Already installed gpio_install_isr_service");
		return ESP_OK;
	}
	LOG_INFO("Installing gpio_install_isr_service");
    esp_err_t err = gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM );
	if(err!=ESP_OK){
		return err;
	}
    isr_service_installed = true;
	return ESP_OK;
}
