#pragma once
#include "./Interfaces/IIncomingMessageHandler.hpp"
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
};