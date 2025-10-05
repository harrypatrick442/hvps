	#ifndef UARTDuplexChannel_hpp
	#define UARTDuplexChannel_hpp
	#include "../../Communication/Interfaces/IIncomingMessageHandler.hpp"
	#include "../../Communication/Interfaces/IMessageSender.hpp"
	#include "../../Logging/Log.hpp"
	#include "../../cJSON/cJSON.h"
	#include "driver/uart.h"
	#include <mutex>
	#include <atomic> 
	#include <memory>
	class UARTDuplexChannel : public IMessageSender{
		private:
			static std::atomic<bool> _uart1Used;
			static std::atomic<bool> _uart2Used;
			std::atomic<IIncomingMessageHandler*> _incomingMessageHandler;
			bool _disposed;
			uart_port_t _uartPort;  // invalid by default
			std::mutex _mutexDispose;
			static std::mutex _mutexClaimReleaseNUart;
			bool _claimedUartPort;
			TaskHandle_t _taskHandle;
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
			bool check_nUARTValidAndSetUartPort(int nUART);
			static void loopTaskEntry(void* obj);
			void loop();
	};
	#endif // UARTDuplexChannel_hpp