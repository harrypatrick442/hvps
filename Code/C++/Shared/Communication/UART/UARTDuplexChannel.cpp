	#include "driver/gpio.h"
	#include "esp_log.h"
	#include "../../Tasks/TaskFactory.hpp"
	#include "UARTDuplexChannel.hpp"
	#include "../../System/Aborter.hpp"
	#include "../../Timing/Delay.hpp"
	#include "../../JSON/CJsonRAII.hpp"
	#include <cstring>

	const char* UARTDuplexChannel::TAG = "UARTDuplexChannel";
	std::atomic<bool> UARTDuplexChannel::_uart1Used{false};
	std::atomic<bool> UARTDuplexChannel::_uart2Used{false};
	std::mutex UARTDuplexChannel::_mutexClaimReleaseNUart;
	UARTDuplexChannel::UARTDuplexChannel(
		/*uart_port_t*/ int nUART,
		/*int*/ int txPin, 
		/*int*/ int rxPin, 
		/*int*/ int baudRate,
		bool invertTx,
		bool invertRx
	) :
		_disposed(false),
		_uartPort(UART_NUM_MAX),
		_claimedUartPort(false),
		_taskHandle(nullptr) {
		
		std::unique_lock<std::mutex> lock(_mutexClaimReleaseNUart);
		if(!check_nUARTValidAndSetUartPort(nUART)){
			_disposed = true;
			return;
		}
		uart_config_t uartConfig = {
			.baud_rate = baudRate,                                 /*int*/
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
			Aborter::safeAbort(TAG, "uart_param_config failed for UART%d: %s (0x%x)",
				_uartPort, esp_err_to_name(err), err);
			return;
		}
		err = uart_set_pin(
			/*uart_num*/  _uartPort,         /*uart_port_t*/
			/*tx_io_num*/ txPin,           /*int*/
			/*rx_io_num*/ rxPin,           /*int*/
			/*rts_io_num*/ UART_PIN_NO_CHANGE, /*int*/
			/*cts_io_num*/ UART_PIN_NO_CHANGE  /*int*/
		);
		if(err!=ESP_OK){
			Aborter::safeAbort(TAG, "uart_set_pin failed for UART%d: %s (0x%x)",
				_uartPort, esp_err_to_name(err), err);
			return;
		}

		err= uart_driver_install(
			/*uart_num*/           _uartPort,  /*uart_port_t*/
			/*rx_buffer_size*/     2048,      /*int*/
			/*tx_buffer_size*/     1024,         /*int (0 disables TX buffer)*/
			/*queue_size*/         0,         /*int (0 disables UART event queue)*/
			/*uart_queue*/         NULL,      /*QueueHandle_t* (null = no event queue)*/
			/*intr_alloc_flags*/   0          /*int (interrupt flags, e.g. ESP_INTR_FLAG_LEVEL1)*/
		);
		uint32_t inversionMask = 0;
		if (invertTx) inversionMask |= UART_SIGNAL_TXD_INV;
		if (invertRx) inversionMask |= UART_SIGNAL_RXD_INV;

		if (inversionMask != 0) {
			Log::Info(TAG, "Applying UART line inversion mask: 0x%02X", inversionMask);
			uart_set_line_inverse(_uartPort, inversionMask);
		}
		if(err!=ESP_OK){
			Aborter::safeAbort(TAG, "uart_driver_install failed for UART%d: %s (0x%x)",
				_uartPort, esp_err_to_name(err), err);
			return;
		}
		Log::Info(TAG, "UART%d is using baud rate %d", _uartPort, baudRate);
		_claimedUartPort = true;
	};
	void UARTDuplexChannel::startThisThread(){
		loop();
	}
	void UARTDuplexChannel::startAsNewNonPriorityTask(){
		TaskFactory::createNonPriorityTask(loopTaskEntry, this, "UART", &_taskHandle);
	}
	UARTDuplexChannel::~UARTDuplexChannel(){
		std::unique_lock<std::mutex> lockClaimReleaseNUart(_mutexClaimReleaseNUart);
		std::unique_lock<std::mutex> lock(_mutexDispose);
		_disposed = true;
		lock.unlock();
		if(_taskHandle!=nullptr){
			while (eTaskGetState(_taskHandle) != eDeleted) {
				Delay::ms(1); // Yield until the loop task has exited
			}
		}
		uart_driver_delete(_uartPort);
		_incomingMessageHandler.exchange(nullptr, std::memory_order_acq_rel);
		if (_claimedUartPort) {
			if (_uartPort == UART_NUM_1) {
				_uart1Used.store(false, std::memory_order_release);
			} else if (_uartPort == UART_NUM_2) {
				_uart2Used.store(false, std::memory_order_release);
			}
		}
	}
	bool UARTDuplexChannel::check_nUARTValidAndSetUartPort(int nUART){
		if (nUART == 1) {
			// exchange returns previous value; false means we just acquired it
			if (!_uart1Used.exchange(true, std::memory_order_acq_rel)) {
				_uartPort = UART_NUM_1;
				return true;
			}
			Aborter::safeAbort(TAG, "UART1 is already in use");
			return false;
		}
		if (nUART == 2) {
			if (!_uart2Used.exchange(true, std::memory_order_acq_rel)) {
				_uartPort = UART_NUM_2;
				return true;
			}
			Aborter::safeAbort(TAG, "UART2 is already in use");
			return false;
		}

		Aborter::safeAbort(TAG, "nUART cannot have value (%d)", nUART);
		return false;
	}
	void UARTDuplexChannel::sendMessage(cJSON* message, bool deleteMessageAfter){
		if (!message) {
			Log::Error(TAG, "UART%d sendMessage: message is null", _uartPort);
			return;
		}
		CJsonRAII cJsonRAII(deleteMessageAfter?message:nullptr);
		// Serialize cJSON object to string
		char* json_str = cJSON_PrintUnformatted(message);  // Or cJSON_Print() if you prefer pretty output
		if (!json_str) {
			Log::Error(TAG, "UART%d sendMessage: failed to serialize JSON", _uartPort);
			return;
		}
		char* json_with_newline = NULL;
		if (asprintf(&json_with_newline, "%s\n", json_str) == -1 || !json_with_newline) {
			Log::Error(TAG, "UART%d sendMessage: asprintf failed", _uartPort);
			free(json_str);
			return;
		}
		free(json_str);
		std::unique_lock<std::mutex> lock(_mutexDispose);
		if(_disposed){
			free(json_with_newline); 
			return;
		}
		//Log::Info(TAG, "uart_write_bytes");
		//Log::Info(TAG, json_with_newline);
		uart_write_bytes(
			/*uart_num*/   _uartPort,   /*uart_port_t*/
			/*src*/        json_with_newline,          /*const char* or const void* */
			/*length*/     strlen(json_with_newline)   /*size_t*/
		);
		uart_wait_tx_done(_uartPort, pdMS_TO_TICKS(50));
		lock.unlock();
		free(json_with_newline); 
	}
	void UARTDuplexChannel::loopTaskEntry(void* obj) {
		UARTDuplexChannel* instance = static_cast<UARTDuplexChannel*>(obj);
		instance->loop();
	}
	void UARTDuplexChannel::loop() {
		char receiveBuffer[256];     // temporary buffer for reading from UART
		char lineBuffer[512];     // assemble one full line here
		size_t lineLength = 0;       // current line length
		bool disgardingTillNewLine = false;
		while (true) {
			std::unique_lock<std::mutex> lock(_mutexDispose);
			if(_disposed){
				break;
			}
			// Read up to N bytes with 100ms timeout
			int len = uart_read_bytes(
				_uartPort,                // uart_port_t
				(uint8_t*)receiveBuffer,     // destination buffer
				sizeof(receiveBuffer),       // max bytes to read
				pdMS_TO_TICKS(100)         // timeout
			);
			lock.unlock();
			if (len <= 0) {
				// Small yield to FreeRTOS
				vTaskDelay(10 / portTICK_PERIOD_MS);
				continue;
			}
			for (int i = 0; i < len; i++) {
				char c = receiveBuffer[i];
				if(disgardingTillNewLine){
					if (c == '\n') {
						disgardingTillNewLine = false;
					}
					continue;
				}
				if (c == '\n'||c=='\0') {
					// Null-terminate and parse
					if(lineLength<1){
						continue;
					}
					lineBuffer[lineLength] = '\0';

					cJSON* json = cJSON_Parse(lineBuffer);
					Log::Info(TAG, "Received line: %s", lineBuffer);  // <-- added print here
					//Log::Info(TAG, "Line length was: %d", lineLength);
					/*for (int i = 0; i<lineLength; i++) {
						Log::Info(TAG, "Char: '%c'  Code: %d\n", lineBuffer[i], (unsigned char)lineBuffer[i]);
					}*/
					if (json) {
						IIncomingMessageHandler* h = _incomingMessageHandler.load(std::memory_order_acquire); // copy shared_ptr atomically
						if(h){
							bool dontDelete = false;
							h->handleIncomingMessage(json, dontDelete);
							if(!dontDelete){
								cJSON_Delete(json);
							}
						}
						else{
							cJSON_Delete(json);
							Log::Error(TAG, "UART%d _incomingMessageHandler not set", _uartPort);
						}
					} else {
						Log::Error(TAG, "UART%d Invalid JSON: %s", _uartPort, lineBuffer);
					}

					// Reset line buffer
					lineLength = 0;
					continue;
				}
				if (lineLength < sizeof(lineBuffer) - 1) {
					lineBuffer[lineLength++] = c;
					continue;
				}
				// Overflowed line buffer
				Log::Error(TAG, "UART%d Line too long — discarding", _uartPort);
				lineLength = 0;
				disgardingTillNewLine = true;
			}
		}
		vTaskDelete(NULL);
	}
	void UARTDuplexChannel::setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) {
		_incomingMessageHandler.store(incomingMessageHandler, std::memory_order_release);
	}
