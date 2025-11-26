#pragma once
#include "../../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../../Communication/Interfaces/IMessageSender.hpp"
#include "../../Logging/Log.hpp"
#include "../../cJSON/cJSON.h"
#include "driver/uart.h"
#include <mutex>
#include <atomic> 
#include <memory>
#include "UARTBase.hpp"
class UARTDuplexChannel : public IMessageSender{
	private:
		std::atomic<IIncomingMessageHandler*> _incomingMessageHandler;
		bool _disposed;
		std::mutex _mutexDispose;
		TaskHandle_t _taskHandle;
		std::unique_ptr<UARTBase> _uart;
	public:
		static const char* TAG;
		UARTDuplexChannel(
			/*uart_port_t*/ int nUART,
			/*int*/ int txPin, 
			/*int*/ int rxPin, 
			/*int*/ int baudRate = 115200,
			bool invertTx = false,
			bool invertRx = false);
		void startThisThread();
		void startAsNewNonPriorityTask();
		void sendMessage(cJSON* message, bool deleteMessageAfter = true) override;
		void setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler);
	protected:	
		virtual ~UARTDuplexChannel();
	private: 
		static void loopTaskEntry(void* obj);
		void loop();
};