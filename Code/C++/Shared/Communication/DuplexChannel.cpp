#include "../Tasks/TaskFactory.hpp"
#include "DuplexChannel.hpp"
#include "System/SafeAbort.hpp"
#include "../Timing/Delay.hpp"
#include "../JSON/CJsonRAII.hpp"
#include "../Logging/Log.hpp"
#include "../Communication/Enums/MessageIntegrity.hpp"
#include "../Core/Checksums/Crc32.hpp"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cctype>
//#include "driver/gpio.h"
//#include "esp_log.h"
const char* DuplexChannel::TAG = "DuplexChannel";
DuplexChannel::DuplexChannel(std::unique_ptr<IChannel> channel) :
	_incomingMessageHandler(nullptr),
	_disposed(false),
	_taskHandle(nullptr),
	_channel(std::move(channel))
{
	_receiveBuffer = new char[256];
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
		LOG_ERROR("%s: sendMessage: message is null", _channel->getDescription());
		return;
	}
	CJsonRAII cJsonRAII(deleteMessageAfter?message:nullptr);
	// Serialize cJSON object to string
	char* json_str = cJSON_PrintUnformatted(message);  // Or cJSON_Print() if you prefer pretty output
	if (!json_str) {
		LOG_ERROR("%s: sendMessage: failed to serialize JSON", _channel->getDescription());
		return;
	}
	char* json_with_newline = NULL;
	
	uint32_t crc32 = Crc32::compute(json_str, strlen(json_str));
	char crc32HexStr[9];
	snprintf(crc32HexStr, sizeof(crc32HexStr), "%08" PRIX32, crc32);
	if (asprintf(&json_with_newline, "%s%s\n", json_str, crc32HexStr) == -1 || !json_with_newline) {
		LOG_ERROR("%s: sendMessage: asprintf failed", _channel->getDescription());
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
				MessageIntegrity messageIntegrity = inspectCRC32AndReturnJSONLength(lineBuffer, lineLength);
				if(lineLength<1){
					continue;
				}
				if(!takeAnyNoiseOffStart(lineBuffer, lineLength)){
					continue;
				}
				lineBuffer[lineLength] = '\0';
				cJSON* json = cJSON_Parse(lineBuffer);
				//LOG_INFO("Received line: %s", lineBuffer);  // <-- added print here
				//LOG_INFO("Line length was: %d", lineLength);
				/*for (int i = 0; i<lineLength; i++) {
					LOG_INFO("Char: '%c'  Code: %d\n", lineBuffer[i], (unsigned char)lineBuffer[i]);
				}*/
				if (json) {
					IIncomingMessageHandler* h = _incomingMessageHandler.load(std::memory_order_acquire); // copy shared_ptr atomically
					if(h){
						bool dontDelete = false;
						h->handleIncomingMessage(json, dontDelete, messageIntegrity);
						if(!dontDelete){
							cJSON_Delete(json);
						}
					}
					else{
						cJSON_Delete(json);
						LOG_ERROR("%s: _incomingMessageHandler not set", _channel->getDescription());
					}
				} else {
					LOG_ERROR("%s: Invalid JSON: %s", _channel->getDescription(), lineBuffer);
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
			LOG_ERROR("%s: Line too long — discarding", _channel->getDescription());
			lineLength = 0;
			disgardingTillNewLine = true;
		}
	}
}
bool DuplexChannel::takeAnyNoiseOffStart(char* lineBuffer, size_t& lineLength){
	if(lineLength<1){
		//The buffer is empty so did not find json.
		return false;
	}
	if(lineBuffer[0]=='{'){
		//Found the start of json right at the beginning of the line
		return true;
	}
	size_t index = 0;
	while(true){
		if(index>=lineLength){
			//Did not find any start of json and reached end so return
			return false;
		}
		char c = lineBuffer[index];
		if(c=='{'){
			break;
		}
		index++;
	}
	if(index<1){
		//Found the start of json right at the beginning of the line
		return true;
	}
	size_t toIndex = 0;
	size_t newLineLength = lineLength - index;
	while(true){
		lineBuffer[toIndex++]=lineBuffer[index++];
		if(index>=lineLength){
			lineLength = newLineLength;
			return true;
		}
	}
}
MessageIntegrity DuplexChannel::inspectCRC32AndReturnJSONLength(char* lineBuffer, size_t& lineLength){
	if(lineLength<10){
		findLengthToEndOfJSONObject(lineBuffer, lineLength);
		return MessageIntegrity::UnableToCheck;
	}
	size_t index = lineLength - 9;
	if(lineBuffer[index++]!='}'){
		findLengthToEndOfJSONObject(lineBuffer, lineLength);
		return MessageIntegrity::UnableToCheck;
	}
	char crc32Chars[8];
	size_t crc32CharIndex = 0;
	while(true){
		char c = lineBuffer[index++];
		if (!isxdigit(c)) {
			findLengthToEndOfJSONObject(lineBuffer, lineLength);
			return MessageIntegrity::UnableToCheck;
		}
		crc32Chars[crc32CharIndex++]=c;
		if(crc32CharIndex>8){
			break;
		}
	}
	lineLength -= 8;
	uint32_t crc32OffEnd = static_cast<uint32_t>(strtoul(crc32Chars, nullptr, 16));
	uint32_t actualCrc32 = Crc32::compute(lineBuffer, lineLength);
	if(crc32OffEnd!=actualCrc32){
		return MessageIntegrity::VerifiedFailed;
	}
	return MessageIntegrity::VerifiedPassed;
}
void DuplexChannel::findLengthToEndOfJSONObject(char* lineBuffer, size_t& lineLength){
	size_t index = lineLength - 1;
	while(true){
		char c = lineBuffer[index];
		if(c=='}'){
			lineLength = index+1;
			return;
		}
		if(index<=0){
			lineLength = 0;
			return;
		}
		index--;
	}
}
void DuplexChannel::setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) {
	_incomingMessageHandler.store(incomingMessageHandler, std::memory_order_release);
}
