#include "HardwareUART.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "System/SafeAbort.hpp"
HardwareUART::HardwareUART(
	int nUART,
	int txPin, 
	int rxPin, 
	int baudRate,
	bool invertTx, 
	bool invertRx
):
UARTBase(nUART, txPin, rxPin, baudRate, invertTx, invertRx),
_uartPort(UART_NUM_MAX){
	
}
HardwareUART::~HardwareUART(){
	if(_uartPort!=UART_NUM_MAX){
		uart_driver_delete(_uartPort);
	}
}
bool HardwareUART::configure(){
	if(!checkNUARTValid(_nUART)){
		return false;
	}
	_uartPort = (_nUART == 1)?UART_NUM_1:UART_NUM_2;
	uart_config_t uartConfig = {
		.baud_rate = _baudRate,                                 /*int*/
		.data_bits = UART_DATA_8_BITS,                          /*uart_word_length_t*/
		.parity    = UART_PARITY_DISABLE,                       /*uart_parity_t*/
		.stop_bits = UART_STOP_BITS_1,                          /*uart_stop_bits_t*/
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE                   /*uart_hw_flowcontrol_t*/
	};

	esp_err_t err = uart_param_config(
		/*uart_num*/     _uartPort,      /*uart_port_t*/
		/*uart_config*/  &uartConfig   /*const uart_config_t* */
	);
	if(err!=ESP_OK){
		SAFE_ABORT("uart_param_config failed for UART%d: %s (0x%x)",
			_uartPort, esp_err_to_name(err), err);
		return false;
	}
	err = uart_set_pin(
		/*uart_num*/  _uartPort,         /*uart_port_t*/
		/*tx_io_num*/ _txPin,           /*int*/
		/*rx_io_num*/ _rxPin,           /*int*/
		/*rts_io_num*/ UART_PIN_NO_CHANGE, /*int*/
		/*cts_io_num*/ UART_PIN_NO_CHANGE  /*int*/
	);
	if(err!=ESP_OK){
		SAFE_ABORT("uart_set_pin failed for UART%d: %s (0x%x)",
			_uartPort, esp_err_to_name(err), err);
		return false;
	}

	err= uart_driver_install(
		/*uart_num*/           _uartPort,  /*uart_port_t*/
		/*rx_buffer_size*/     2048,      /*int*/
		/*tx_buffer_size*/     1024,         /*int (0 disables TX buffer)*/
		/*queue_size*/         0,         /*int (0 disables UART event queue)*/
		/*uart_queue*/         NULL,      /*QueueHandle_t* (null = no event queue)*/
		/*intr_alloc_flags*/   0          /*int (interrupt flags, e.g. ESP_INTR_FLAG_LEVEL1)*/
	);
	if(err!=ESP_OK){
		SAFE_ABORT("uart_driver_install failed for UART%d: %s (0x%x)",
			_uartPort, esp_err_to_name(err), err);
		return false;
	}
	uint32_t inversionMask = 0;
	if (_invertTx) inversionMask |= UART_SIGNAL_TXD_INV;
	if (_invertRx) inversionMask |= UART_SIGNAL_RXD_INV;

	if (inversionMask != 0) {
		uart_set_line_inverse(_uartPort, inversionMask);
	}
	if(err!=ESP_OK){
		SAFE_ABORT("uart_driver_install failed for UART%d: %s (0x%x)",
			_uartPort, esp_err_to_name(err), err);
		return false;
	}
	return true;
}
size_t HardwareUART::readBytes(char* receiveBuffer, size_t maxlen, uint32_t timeoutMs){
	int res = uart_read_bytes(
		_uartPort,// uart_port_t
		(uint8_t*)receiveBuffer,// destination buffer
		maxlen,// max bytes to read
		pdMS_TO_TICKS(timeoutMs)// timeout
	);
	if(res<0){
		return 0;
	}
	return static_cast<size_t>(res);
}
size_t HardwareUART::writeBytes(const char* jsonWithNewLine, size_t length){
	int res = uart_write_bytes(
		/*uart_num*/   _uartPort,   /*uart_port_t*/
		/*src*/        jsonWithNewLine,          /*const char* or const void* */
		/*length*/     length   /*size_t*/
	);
	if(res<0){
		return 0;
	}
	return static_cast<size_t>(res);
}
void HardwareUART::flushTx(){
	uart_wait_tx_done(_uartPort, pdMS_TO_TICKS(50));
}