#pragma once
#include "./Interfaces/IIncomingMessageHandler.hpp"
#include "./Enums/MessageIntegrity.hpp"
#include "./Interfaces/IChannel.hpp"
#include "./Interfaces/IDuplexChannel.hpp"
#include "./Events/ChannelEvents.hpp"
#include "../cJSON/cJSON.h"
//#include "driver/uart.h"
//#include "driver/gpio.h"
//#include "esp_log.h"
#include <mutex>
#include <atomic> 
#include <memory>
class DuplexChannel : public IDuplexChannel, public ChannelEvents{
	private:
		std::atomic<IIncomingMessageHandler*> _incomingMessageHandler;
		bool _disposed;
		std::mutex _mutexDispose;
		TaskHandle_t _taskHandle;
		std::unique_ptr<IChannel> _channel;
		size_t _receiveBufferSize;
		char* _receiveBuffer;
	public:
		static const char* TAG;
		DuplexChannel(std::unique_ptr<IChannel> channel);
		void startThisThread();
		void startAsNewNonPriorityTask();
		void sendMessage(cJSON* message, bool deleteMessageAfter = true) override;
		void setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler);
	protected:	
		virtual ~DuplexChannel();
	private: 
		void loop();
		
		/**
		 * @brief Removes any leading noise before the start of a JSON object.
		 *
		 * Scans the buffer for the first '{' character. If found, shifts the buffer
		 * in-place so that the JSON object begins at index 0 and updates lineLength
		 * accordingly.
		 *
		 * @param lineBuffer Pointer to the character buffer containing incoming data.
		 * @param lineLength [in,out] Length of valid data in the buffer.
		 * @return true if a JSON start character was found; false otherwise.
		 */
		bool takeAnyNoiseOffStart(char* lineBuffer, size_t& lineLength);
		MessageIntegrity inspectCRC32AndReturnJSONLength(char* lineBuffer, size_t& lineLength);
		void findLengthToEndOfJSONObject(char* lineBuffer, size_t& lineLength);
};