#include "../../Tasks/TaskFactory.hpp"
#include "UARTDuplexChannel.hpp"
#include "../../System/Aborter.hpp"
#include "../../Timing/Delay.hpp"
#include "../../JSON/CJsonRAII.hpp"
#include "HardwareUART.hpp"
#include "SoftwareUART.hpp"
#include <cstring>
//#include "driver/gpio.h"
//#include "esp_log.h"
const char* UARTDuplexChannel::TAG = "UARTDuplexChannel";
UARTDuplexChannel::UARTDuplexChannel(
	/*uart_port_t*/ int nUART,
	/*int*/ int txPin, 
	/*int*/ int rxPin, 
	/*int*/ int baudRate,
	bool invertTx,
	bool invertRx
) :
	_incomingMessageHandler(nullptr),
	_disposed(false),
	_taskHandle(nullptr),
	_uart(nullptr)
{
	_uart = (nUART < 3)
		? std::unique_ptr<UARTBase>(new HardwareUART(nUART, txPin, rxPin, baudRate, invertTx, invertRx))
		: std::unique_ptr<UARTBase>(new SoftwareUART(nUART, txPin, rxPin, baudRate, invertTx, invertRx));
	if(!_uart->configure()){
		_disposed = true;
		return;
	}
};
void UARTDuplexChannel::startThisThread(){
	loop();
}
void UARTDuplexChannel::startAsNewNonPriorityTask(){
	TaskFactory::createNonPriorityTask([this](){
		loop();
	},  "UART", &_taskHandle);
}
UARTDuplexChannel::~UARTDuplexChannel(){
	std::unique_lock<std::mutex> lock(_mutexDispose);
	_disposed = true;
	lock.unlock();
	if(_taskHandle!=nullptr){
		while (eTaskGetState(_taskHandle) != eDeleted) {
			Delay::ms(1); // Yield until the loop task has exited
		}
	}
	_incomingMessageHandler.exchange(nullptr, std::memory_order_acq_rel);
}
void UARTDuplexChannel::sendMessage(cJSON* message, bool deleteMessageAfter){
	if (!message) {
		Log::Error(TAG, "UART%d sendMessage: message is null", _uart->getNUART());
		return;
	}
	CJsonRAII cJsonRAII(deleteMessageAfter?message:nullptr);
	// Serialize cJSON object to string
	char* json_str = cJSON_PrintUnformatted(message);  // Or cJSON_Print() if you prefer pretty output
	if (!json_str) {
		Log::Error(TAG, "UART%d sendMessage: failed to serialize JSON", _uart->getNUART());
		return;
	}
	char* json_with_newline = NULL;
	if (asprintf(&json_with_newline, "%s\n", json_str) == -1 || !json_with_newline) {
		Log::Error(TAG, "UART%d sendMessage: asprintf failed", _uart->getNUART());
		free(json_str);
		return;
	}
	free(json_str);
	std::unique_lock<std::mutex> lock(_mutexDispose);
	if(_disposed){
		free(json_with_newline); 
		return;
	}
	_uart->writeBytes(
		json_with_newline,
		strlen(json_with_newline)
	);
	_uart->flushTx();
	lock.unlock();
	free(json_with_newline); 
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
		int len = _uart->readBytes(
			receiveBuffer,     // destination buffer
			sizeof(receiveBuffer),       // max bytes to read
			100         // timeout
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
						Log::Error(TAG, "UART%d _incomingMessageHandler not set", _uart->getNUART());
					}
				} else {
					Log::Error(TAG, "UART%d Invalid JSON: %s", _uart->getNUART(), lineBuffer);
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
			Log::Error(TAG, "UART%d Line too long — discarding", _uart->getNUART());
			lineLength = 0;
			disgardingTillNewLine = true;
		}
	}
	vTaskDelete(NULL);
}
void UARTDuplexChannel::setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) {
	_incomingMessageHandler.store(incomingMessageHandler, std::memory_order_release);
}
