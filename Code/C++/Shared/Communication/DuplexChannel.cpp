#include "../Tasks/TaskFactory.hpp"
#include "DuplexChannel.hpp"
#include "../System/Aborter.hpp"
#include "../Timing/Delay.hpp"
#include "../JSON/CJsonRAII.hpp"
#include "../Logging/Log.hpp"
#include <cstring>
//#include "driver/gpio.h"
//#include "esp_log.h"
const char* DuplexChannel::TAG = "DuplexChannel";
DuplexChannel::DuplexChannel(std::unique_ptr<IChannel> channel) :
	_incomingMessageHandler(nullptr),
	_disposed(false),
	_taskHandle(nullptr),
	_channel(std::move(channel))
{
	_receiveBufferSize = _channel->getMinRequiredReceiveBufferSize();
	if(_receiveBufferSize<256){
		_receiveBufferSize = 256;
	}
	_receiveBuffer = new char[_receiveBufferSize];
	if(!_channel->configure()){
		_disposed = true;
		return;
	}
};
void DuplexChannel::startThisThread(){
	loop();
}
void DuplexChannel::startAsNewNonPriorityTask(){
	TaskFactory::createNonPriorityTask([this](){
		loop();
	},  "DuplexChannel::startAsNewNonPriorityTask", &_taskHandle);
}
DuplexChannel::~DuplexChannel(){
	std::unique_lock<std::mutex> lock(_mutexDispose);
	_disposed = true;
	lock.unlock();
	if(_taskHandle!=nullptr){
		while (eTaskGetState(_taskHandle) != eDeleted) {
			Delay::ms(1); // Yield until the loop task has exited
		}
	}
	_incomingMessageHandler.exchange(nullptr, std::memory_order_acq_rel);
	delete[] _receiveBuffer;
}
void DuplexChannel::sendMessage(cJSON* message, bool deleteMessageAfter){
	if (!message) {
		Log::Error(TAG, "%s: sendMessage: message is null", _channel->getDescription());
		return;
	}
	CJsonRAII cJsonRAII(deleteMessageAfter?message:nullptr);
	// Serialize cJSON object to string
	char* json_str = cJSON_PrintUnformatted(message);  // Or cJSON_Print() if you prefer pretty output
	if (!json_str) {
		Log::Error(TAG, "%s: sendMessage: failed to serialize JSON", _channel->getDescription());
		return;
	}
	char* json_with_newline = NULL;
	if (asprintf(&json_with_newline, "%s\n", json_str) == -1 || !json_with_newline) {
		Log::Error(TAG, "%s: sendMessage: asprintf failed", _channel->getDescription());
		free(json_str);
		return;
	}
	free(json_str);
	std::unique_lock<std::mutex> lock(_mutexDispose);
	if(_disposed){
		free(json_with_newline); 
		return;
	}
	_channel->writeBytes(
		json_with_newline,
		strlen(json_with_newline)
	);
	_channel->flushTx();
	lock.unlock();
	free(json_with_newline); 
}
void DuplexChannel::loop() {
	char lineBuffer[512];     // assemble one full line here
	size_t lineLength = 0;       // current line length
	bool disgardingTillNewLine = false;
	while (true) {
		std::unique_lock<std::mutex> lock(_mutexDispose);
		if(_disposed){
			break;
		}
		// Read up to N bytes with 100ms timeout
		size_t len = _channel->readBytes(
			_receiveBuffer,     // destination buffer
			_receiveBufferSize,       // max bytes to read
			100         // timeout
		);
		lock.unlock();
		if (len <= 0) {
			// Small yield to FreeRTOS
			vTaskDelay(10 / portTICK_PERIOD_MS);
			continue;
		}
		for (size_t i = 0; i < len; i++) {
			char c = _receiveBuffer[i];
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
						Log::Error(TAG, "%s: _incomingMessageHandler not set", _channel->getDescription());
					}
				} else {
					Log::Error(TAG, "%s: Invalid JSON: %s", _channel->getDescription(), lineBuffer);
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
			Log::Error(TAG, "%s: Line too long — discarding", _channel->getDescription());
			lineLength = 0;
			disgardingTillNewLine = true;
		}
	}
}
void DuplexChannel::setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) {
	_incomingMessageHandler.store(incomingMessageHandler, std::memory_order_release);
}
