#include "sdkconfig.h"  
#ifdef CONFIG_BT_ENABLED 
#include "./Bluetooth_BR_EDR.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_defs.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"
#include "../../Logging/Log.hpp"
#include "../../JSON/CJsonRAII.hpp"
#include "System/SafeAbort.hpp"
#include <cstring>
#define MAX_BT_SEND_QUEUE 10
Bluetooth& Bluetooth::initialize(
    const char* deviceName,
     const char* serverName) {
    if (_instance != nullptr) {
        SAFE_ABORT("Bluetooth already initialized");
        //^^noreturn^^
    }
    _instance = new Bluetooth(deviceName, serverName);
    LOG_INFO("Initializing Classic Bluetooth SPP...");
	esp_err_t ret;
    // Enable Bluetooth Controller
    ret = esp_bt_controller_mem_release(ESP_BT_MODE_BLE); // Disable BLE if not needed
    if (ret) {
        LOG_WARN("BLE Memory Release Failed");
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        SAFE_ABORT("BT Controller Init Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        SAFE_ABORT("BT Controller Enable Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        SAFE_ABORT("Bluedroid Init Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }
    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        SAFE_ABORT("Bluedroid Enable Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }

    // Set device name AFTER Bluedroid is enabled
    if((ret = esp_bt_gap_set_device_name(deviceName))!=ESP_OK){
        SAFE_ABORT("Set Device Name Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }
    LOG_INFO("Set device name to %s", deviceName);

    // Register GAP callback for Secure Connections
    if((ret = esp_bt_gap_register_callback(esp_gap_callback_static))!=ESP_OK){
        SAFE_ABORT("GAP Register Callback Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }
    LOG_INFO("GAP Callback registered for Secure Connections.");

    // Enable Secure Simple Pairing (SSP) with Secure Connections (SC)
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO; // Enables Numeric Comparison & Passkey
    if((ret = esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t)))!=ESP_OK){
        SAFE_ABORT("Set Security Param Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }
    LOG_INFO("Secure Connections (SC) enabled. Numeric Comparison & Passkey authentication.");

    /*uint32_t auth_timeout_ms = 60000;  // Set to 60 seconds (60000 ms)
    if ((ret = esp_bt_gap_set_security_param(ESP_BT_GAP_, &auth_timeout_ms, sizeof(uint32_t))) != ESP_OK) {
        LOG_ERROR("Set Authentication Timeout Failed: %s", esp_err_to_name(ret));
        abort();
        return;
    }*/
    LOG_INFO("Increased authentication timeout to 60 seconds.");


    // Register SPP callback
    if ((ret = esp_spp_register_callback(esp_spp_callback_static)) != ESP_OK) {
        SAFE_ABORT("SPP Register Callback Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }

    // Initialize SPP
    esp_spp_cfg_t spp_cfg = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true, // Enable Enhanced Retransmission Mode (ERTM)
        .tx_buffer_size = 0        // Use default TX buffer size
    };
    if ((ret = esp_spp_enhanced_init(&spp_cfg))!=ESP_OK){
        SAFE_ABORT("SPP Init Failed: %s", esp_err_to_name(ret));
        //^^noreturn^^
    }

    LOG_INFO("Bluetooth SPP Ready. Connect using a Bluetooth terminal.");
    LOG_INFO("Bluetooth initialized. Device name: %s", deviceName);
	return *Bluetooth::_instance;
}

Bluetooth& Bluetooth::getInstance() {
    return *Bluetooth::_instance;
}

const char* Bluetooth::TAG = "Bluetooth";
Bluetooth* Bluetooth::_instance = nullptr;

Bluetooth::Bluetooth(
    const char* deviceName,
    const char* serverName) :
    _deviceName(deviceName),
    _serverName(serverName),
    _incomingMessageHandler(nullptr),
	_incomingMessageWorker(std::make_shared<TransientWorker>(32, 1000, true))
{
	_connectionHandle=0; 
		/*_mainTaskHandle = */
	xTaskGetCurrentTaskHandle();
}

// Secure Connections (SC) GAP Callback

void Bluetooth::esp_gap_callback_static(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    if (_instance == nullptr) {
        LOG_ERROR("Bluetooth instance is NULL");
		return;
    }
    _instance->esp_gap_callback(event, param);
}
void Bluetooth::esp_gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    esp_err_t ret;
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
                LOG_INFO("Pairing successful with device: %s", param->auth_cmpl.device_name);
            } else {
                LOG_ERROR("Pairing failed. Error Code: %d", param->auth_cmpl.stat);
            }
            break;

        case ESP_BT_GAP_CFM_REQ_EVT:  // Numeric Comparison
            LOG_INFO("Confirm pairing with PIN: %d", param->cfm_req.num_val);
            if((ret = esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true))!=ESP_OK){
                SAFE_ABORT("Confirm Reply Failed: %s", esp_err_to_name(ret));
                return;
            }
            LOG_INFO("Pairing confirmed.");
            break;

        case ESP_BT_GAP_KEY_NOTIF_EVT:  // Show passkey for manual entry
            LOG_INFO("Enter this passkey on the client: %06d", param->key_notif.passkey);
            break;

        case ESP_BT_GAP_KEY_REQ_EVT:  // Passkey entry requested (only for legacy devices)
            LOG_INFO("Passkey entry requested. Auto-entering passkey.");
            if((ret = esp_bt_gap_pin_reply(param->key_req.bda, true, 6, (uint8_t*)"123456"))!=ESP_OK){
                SAFE_ABORT("PIN Reply Failed: %s", esp_err_to_name(ret));
                return;
            }
            break;
        case ESP_BT_GAP_ACL_CONN_CMPL_STAT_EVT:
            LOG_INFO("ACL Connection Complete.");
            break;
        case ESP_BT_GAP_ENC_CHG_EVT:
            LOG_INFO("Encryption Changed.");
            break;
        case ESP_BT_GAP_MODE_CHG_EVT:
            LOG_INFO("Mode Changed.");
            break;
        default:
            LOG_INFO("Unhandled GAP event: %d", event);
            break;
    }
}

void Bluetooth::esp_spp_callback_static(
    esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    if (_instance == nullptr) {
        LOG_ERROR("Bluetooth instance is NULL");
		return;
    }
    _instance->esp_spp_callback(event, param);
}
void Bluetooth::esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    esp_err_t ret;
    switch (event) {
        case ESP_SPP_INIT_EVT:
            LOG_INFO("SPP Initialized, making ESP32 discoverable with server name %s", _instance->_serverName);
            if((ret = esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE))!=ESP_OK){
                SAFE_ABORT("Set Scan Mode Failed: %s", esp_err_to_name(ret));
                return;
            }
            if((ret = esp_spp_start_srv(ESP_SPP_SEC_AUTHENTICATE, 
				ESP_SPP_ROLE_SLAVE, 0, _instance->_serverName))!=ESP_OK){
                SAFE_ABORT("Start SPP Server Failed: %s", esp_err_to_name(ret));
                return;
            }
            break;
		case ESP_SPP_DISCOVERY_COMP_EVT:
			LOG_INFO("ESP_SPP_DISCOVERY_COMP_EVT, status=%d scn_num=%d", param->disc_comp.status, param->disc_comp.scn_num);
			
			break;
		case ESP_SPP_START_EVT:
			LOG_INFO("SPP server started, handle = %d", param->start.handle);
			break;
		case ESP_SPP_CONG_EVT:
			LOG_INFO("SPP Congestion status: %d", param->cong.cong);
			break;
        case ESP_SPP_SRV_OPEN_EVT :
            _connectionHandle = param->open.handle;
			_mutexWrite.lock();
			_writeInProgress = false;
			_mutexWrite.unlock();
			emptyOutgoingQueue();
			dispatchOnOpened();
			tryFlushSendQueue();
            break;

        case ESP_SPP_CLOSE_EVT:
			_connectionHandle = 0;
			dispatchOnClosed();
			emptyOutgoingQueue();
            break;

        case ESP_SPP_DATA_IND_EVT:
			{
				//LOG_INFO("Received %d bytes: %.*s", param->data_ind.len, param->data_ind.len, param->data_ind.data);
				
				cJSON *root = cJSON_ParseWithLength((const char*)param->data_ind.data, param->data_ind.len);
				if(_incomingMessageWorker->enqueue([this, root]{ 
					if(_incomingMessageHandler==nullptr){
						LOG_WARN("Incoming Message Handler is NULL. Cannot handle incoming message.");
						return;
					}
					bool dontDelete = false;
					_incomingMessageHandler->handleIncomingMessage(root, dontDelete, MessageIntegrity::Unverified);
					if(!dontDelete){
						cJSON_Delete(root);
					}
				})){
					break;
				}
				cJSON_Delete(root);
				break;
			}
		case ESP_SPP_WRITE_EVT:
            //LOG_INFO("Data Sent: %d bytes", param->write.len);
			_mutexWrite.lock();
			if (!_outgoingQueue.empty()) {
				char* sentMsg = _outgoingQueue.front();
				_outgoingQueue.pop();
				free(sentMsg);
				// ✅ Important: free the memory after it's sent
			}
			_writeInProgress = false;
			_mutexWrite.unlock();
			tryFlushSendQueue();  // Continue sending next message if any
			break;
        default:
            break;
    }
}
void Bluetooth::setIncomingMessageHandler(IIncomingMessageHandler* incomingMessageHandler) {
    _incomingMessageHandler = incomingMessageHandler;
}
void Bluetooth::sendMessage(cJSON* message, bool deleteMessageAfter) {
    if (!message) {
        LOG_ERROR("sendMessage: message is null");
        return;
    }
	CJsonRAII cJsonRAII(deleteMessageAfter?message:nullptr);
    // Serialize cJSON object to string
    char* json_str = cJSON_PrintUnformatted(message);  // Or cJSON_Print() if you prefer pretty output
    if (!json_str) {
        LOG_ERROR("sendMessage: failed to serialize JSON");
        return;
    }
	if(_connectionHandle==0){
		LOG_WARN("Called sendMessage while not connected");
		return;
	}
    if (_outgoingQueue.size() >= MAX_BT_SEND_QUEUE) {
        LOG_ERROR("Bluetooth TX queue overflow! Dropping message.");
        free(json_str);
        return;
    }

	
	char* json_with_newline = NULL;
	if (asprintf(&json_with_newline, "%s\n", json_str) == -1 || !json_with_newline) {
		LOG_ERROR("sendMessage: asprintf failed");
		free(json_str);
		return;
	}
	free(json_str); 
    _outgoingQueue.push(json_with_newline);
    tryFlushSendQueue();  // Attempt to send immediately if idle
}
void Bluetooth::tryFlushSendQueue() {
    if (_connectionHandle==0){
        LOG_WARN("_connectionHandle not set at this time...");
		return;
	}
    _mutexWrite.lock();
	if(_writeInProgress||_outgoingQueue.empty()){
		_mutexWrite.unlock();
        return;
	}	
    _writeInProgress = true;
	_mutexWrite.unlock();
	//LOG_INFO("There are %zu entries in queue", _outgoingQueue.size());
    char* nextMsg = _outgoingQueue.front();
    size_t len = strlen(nextMsg);
    esp_err_t ret = esp_spp_write(_connectionHandle, len, (uint8_t*)nextMsg);
    if (ret == ESP_OK) {
    } else {
		_mutexWrite.lock();
        _writeInProgress = false;
		_mutexWrite.unlock();
        LOG_ERROR("esp_spp_write failed: %s", esp_err_to_name(ret));
    }
}
void Bluetooth::emptyOutgoingQueue(){
	while(!_outgoingQueue.empty()) {
		char* sentMsg = _outgoingQueue.front();
		_outgoingQueue.pop();
		free(sentMsg);
		// ✅ Important: free the memory after it's sent
	}
}
#endif