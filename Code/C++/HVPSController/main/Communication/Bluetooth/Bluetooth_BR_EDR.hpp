#ifndef Bluetooth_BR_EDR_hpp
#define Bluetooth_BR_EDR_hpp
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "../../Logging/Log.hpp"
#include "esp_spp_api.h"
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_bt_api.h>
#include <esp_spp_api.h>
#include <esp_log.h>
#include "../../Communication/Interfaces/IChannel.hpp"
#include "../../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../../cJSON/cJSON.h"
#include <queue>
#include <mutex>
class Bluetooth : public IChannel{
    public:
        static Bluetooth& initialize(
            const char* deviceName, 
            const char* serverName);

        // Get the singleton instance of the I2C class
        static Bluetooth& getInstance();
        void setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) override;
			void sendMessage(cJSON* message, bool deleteMessageAfter = true) override;
    private:
        Bluetooth(
            const char* deviceName, 
            const char* serverName);
            uint32_t _connectionHandle;
			std::queue<char*> _outgoingQueue;
			volatile bool _writeInProgress = false;
			std::mutex _mutexWrite;
        // Delete copy constructor and assignment operator
        Bluetooth(const Bluetooth&) = delete;
        Bluetooth& operator=(const Bluetooth&) = delete;

        static Bluetooth* _instance;
        static const char* TAG;
        const char* _deviceName;
        const char* _serverName;
        IIncomingMessageHandler* _incomingMessageHandler;
        static void esp_spp_callback_static(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
        void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
        static void esp_gap_callback_static(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
        void esp_gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
        
		void tryFlushSendQueue();
};
#endif // Bluetooth_BR_EDR_hpp