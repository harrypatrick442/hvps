#include "./Bluetooth_BLE.hpp"
#include <array>
#include <cstring>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_pm.h"
#include "esp_spp_api.h"
#include "../../Logging/Log.hpp"
#include "../../Storage/Flash.hpp"
#include "../../System/Aborter.hpp"
#include "../../System/WatchdogFeeder.hpp"

Bluetooth_BLE* Bluetooth_BLE::_instance = nullptr;
const char* Bluetooth_BLE::TAG = "Bluetooth";

Bluetooth_BLE& Bluetooth_BLE::getInstance(){
    if(_instance==nullptr){
        Log::Error(TAG, "Bluetooth not initialized. Call initialize() first.");
        Aborter::run();
    }
    return *Bluetooth_BLE::_instance;

}
// Modify constructor to accept UUIDs
Bluetooth_BLE::Bluetooth_BLE(
    const char* deviceName, 
    const char* serverName,
    std::array<uint8_t, ESP_UUID_LEN_128>& serviceUUID,
    uint16_t characteristicUUID,
    DelegateFeedWatchdog& feedWatchdog) :
    _deviceName(deviceName),
    _serverName(serverName),
    _serviceUUID(serviceUUID),
    _characteristicUUID(characteristicUUID),
    _feedWatchdog(feedWatchdog) {
    service_handle = 0;
    characteristic_value = "{\"status\":\"ok\"}";
    ESP_LOG_BUFFER_HEX("Service UUID", _serviceUUID.data(), ESP_UUID_LEN_128);
    Log::Info(TAG, "Characteristics UUID: %d", characteristicUUID);
    
    // Initialize ESP UUID structure
    esp_service_uuid.len = ESP_UUID_LEN_128;
    memcpy(esp_service_uuid.uuid.uuid128, _serviceUUID.data(), ESP_UUID_LEN_128);
    esp_char_uuid.len = ESP_UUID_LEN_16;
    esp_char_uuid.uuid.uuid16 = _characteristicUUID;
    
    _feedWatchdog();
    Log::Info(TAG, "sizeof is: %d", sizeof(_serviceUUID));
    Log::Info(TAG, "sizeof is: %d", sizeof(_characteristicUUID));
    service_table = new esp_gatts_attr_db_t[2]{
        { {ESP_GATT_AUTO_RSP}, 
            {ESP_UUID_LEN_128, _serviceUUID.data(), ESP_GATT_PERM_READ|ESP_GATT_PERM_WRITE, 
            sizeof(_serviceUUID), sizeof(_serviceUUID), _serviceUUID.data()} 
        }
    };
    /*
    */
    
    _feedWatchdog();
    if (!service_table) {
        Log::Error(TAG, "service_table allocation failed!");
        Aborter::run(); // Prevent further execution
    }
    adv_params = new esp_ble_adv_params_t{
        .adv_int_min = 0x20,
        .adv_int_max = 0x40,
        .adv_type = ADV_TYPE_IND,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .peer_addr          = {0},     // No specific peer address (empty)
        .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC,  // Address type of the peer device (e.g., public)
        .channel_map = ADV_CHNL_ALL,
        .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        /*
        .adv_int_min        = 0x20,    // Minimum advertising interval
        .adv_int_max        = 0x40,    // Maximum advertising interval
        .adv_type           = ADV_TYPE_IND,  // Type of advertisement (ADV_TYPE_IND is an "indirect" advertisement)
        .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,  // Type of address for the device
        .channel_map        = ADV_CHNL_ALL,  // Which Bluetooth channels to use for advertising
        .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY  // Advertisement filter policy (no restrictions)
        */
    };
    _feedWatchdog();
    if (!adv_params) {
        Log::Error(TAG, "adv_params allocation failed!");
        Aborter::run();
    }
    adv_data = new esp_ble_adv_data_t{
        .set_scan_rsp = false,// ✅ Not a scan response
        .include_name = true,// ✅ Include device name
        .include_txpower = true,// ✅ Include transmit power
        .min_interval = 0x20,// ✅ Minimum interval
        .max_interval = 0x40,// ✅ Maximum interval
        .appearance = 0x00,// ✅ No appearance
        .manufacturer_len = 0,// ✅ No manufacturer
        .p_manufacturer_data = NULL,//  ✅ No manufacturer
        .service_data_len = 0,// ✅ No service data
        .p_service_data = NULL,// ✅ No service data
        //.service_uuid_len = 16,  // ✅ Explicit 16-byte length
        //.p_service_uuid = _serviceUUID.data(),// ✅ Service UUID
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),// ✅ Flags
    };
    Log::Info(TAG, "The size of adv_data is: %d", sizeof(adv_data));
    if (!adv_data) {
        Log::Error(TAG, "adv_data allocation failed!");
        Aborter::run();
    }
    _feedWatchdog();
    log_adv_data(adv_data);
    _feedWatchdog();
    log_adv_params(adv_params);
    _feedWatchdog();
    log_service_table(service_table);
    _feedWatchdog();
}

// Initialization method
void Bluetooth_BLE::initialize(
    const char* deviceName, 
    const char* serverName,
    std::array<uint8_t, ESP_UUID_LEN_128>& serviceUUID,
    uint16_t characteristicUUID,
    DelegateFeedWatchdog& feedWatchdog) {

    if (_instance != nullptr) {
        Log::Error(TAG, "Bluetooth already initialized");
        Aborter::run();
    }

    _instance = new Bluetooth_BLE(deviceName, 
        serverName, serviceUUID, characteristicUUID,
        feedWatchdog);
    Flash::initialize();
    Log::Info(TAG, "Initializing Bluetooth...");esp_pm_lock_handle_t pm_lock;
    esp_err_t err = esp_pm_lock_create(ESP_PM_APB_FREQ_MAX, 0, "disable_sleep", &pm_lock);
    if (err == ESP_OK) {
        esp_pm_lock_acquire(pm_lock);
        printf("✅ Power management lock acquired (prevents some sleep modes)\n");
    }
    printf("Sleep is now disabled (PM Lock Active)\n");
    // Enable Bluetooth Controller
    /*esp_err_t ret = esp_bt_controller_mem_release(ESP_BT_MODE_BLE); // Disable BLE if not needed
    if (ret) {
        Log::Warn(TAG, "BLE Memory Release Failed");
    }*/
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        Log::Error(TAG, "BT Controller Init Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BLE)) != ESP_OK) {
        Log::Error(TAG, "BT Controller Enable Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        Log::Error(TAG, "Bluedroid Init Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        Log::Error(TAG, "Bluedroid Enable Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }

    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        Log::Error(TAG, "Bluedroid not enabled!");
        Aborter::run();
        return;
    }
    // Set device name AFTER Bluedroid is enabled
    if ((ret = esp_ble_gap_set_device_name(deviceName)) != ESP_OK) {
        Log::Error(TAG, "Set Device Name Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }
    Log::Info(TAG, "Setting device name to %s", deviceName);
    // Register GAP and GATT event handlers
   if ((ret = esp_ble_gap_register_callback(gap_event_handler_static)) != ESP_OK) {
        Log::Error(TAG, "GAP Register Callback Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }
    if ((ret = esp_ble_gatts_register_callback(gatt_event_handler_static)) != ESP_OK) {
        Log::Error(TAG, "GATTS Register Callback Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }
    if ((ret = esp_ble_gatts_app_register(0)) != ESP_OK) {
        Log::Error(TAG, "GATT App Register Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }
    // Start BLE Advertising
    if ((ret = esp_ble_gap_config_adv_data(_instance->adv_data)) != ESP_OK) {
        Log::Error(TAG, "Config Adv Data Failed: %s", esp_err_to_name(ret));
        Aborter::run();
        return;
    }

    Log::Info(TAG, "Bluetooth Initialized and ready!");
}
void Bluetooth_BLE::checkInitialized(){
    if(_instance != nullptr) {
        return;
    }
    Log::Error(Bluetooth_BLE::TAG, "Bluetooth instance is null");
    Aborter::run();
}
// GAP Event Handler (Advertising & Scanning)
void Bluetooth_BLE::gap_event_handler_static(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    Bluetooth_BLE::checkInitialized();
    _instance->gap_event_handler(event, param);
}

// GATT Event Handler
void Bluetooth_BLE::gatt_event_handler_static(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    Bluetooth_BLE::checkInitialized();
    _instance->gatt_event_handler(event, gatts_if, param);
}

// GAP Event Handler (Advertising & Scanning)
void Bluetooth_BLE::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    _feedWatchdog();
    Log::Info(TAG, "GAP Event Handler called with event: %d", event);
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            Log::Info(TAG, "Advertisement Data Set. Starting Advertising...");
            vTaskDelay(pdMS_TO_TICKS(500));
            if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
                Log::Error(TAG, "Bluedroid not enabled before advertising!");
                return;
            }
            startAdvertising();
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                Log::Error(TAG, "Advertising start failed");
            } else {
                Log::Info(TAG, "Advertising start successfully");
            }
            break;
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
            Log::Info(TAG, "Scan parameters set.");
            break;
        case ESP_GAP_BLE_SCAN_RESULT_EVT:
            Log::Info(TAG, "Scan result event.");
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            Log::Info(TAG, "Advertising stopped.");
            break;
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            Log::Info(TAG, "Connection parameters updated.");
            break;
        case ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT:
            Log::Info(TAG, "Packet length set.");
            break;
        default:
            Log::Debug(TAG, "Unhandled GAP event: %d", event);
            break;
    }
    _feedWatchdog();
}

// GATT Event Handler
void Bluetooth_BLE::gatt_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    _feedWatchdog();
    esp_err_t ret;
    switch (event) {
        case ESP_GATTS_REG_EVT:     /*!< When register application id, the event comes */
            {
                Log::Info(TAG, "GATT Server Registered.");
                esp_gatt_srvc_id_t service_id = {
                    .id = {
                        .uuid = esp_service_uuid,
                        .inst_id = 0, // Initialize inst_id
                    },
                    .is_primary = true
                };
                ret = esp_ble_gatts_create_service(gatts_if,
                 &service_id, 1); // Create Service
                if (ret != ESP_OK) {
                    Log::Error(TAG, "Failed to create service: %s", esp_err_to_name(ret));
                    // Handle the error appropriately
                    return;
                }
                break;
            }
            break;
        case ESP_GATTS_READ_EVT:      /*!< When gatt client request read operation, the event comes */
            {  
                Log::Info(TAG, "BLE Read Received!");
                handle_read_event(gatts_if, param);
            }
            break;
        case ESP_GATTS_WRITE_EVT:/*!< When gatt client request write operation, the event comes */
            {
                Log::Info(TAG, "BLE Write Received!");

                // Store the received JSON string
                characteristic_value = std::string((char*)param->write.value, param->write.len);

                Log::Info(TAG, "Updated Characteristic Value: %s", characteristic_value.c_str());

                // Acknowledge the write
                esp_gatt_rsp_t rsp;
                memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
                rsp.attr_value.handle = param->write.handle;
                rsp.attr_value.len = 0; // No data to send back

                esp_ble_gatts_send_response(
                    gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, &rsp
                );
            }
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:/*!< When gatt client request execute write, the event comes */
            Log::Info(TAG, "Execute Write event.");
            break;
        case ESP_GATTS_MTU_EVT:/*!< When set mtu complete, the event comes */
            Log::Info(TAG, "MTU event.");
            break;
        case ESP_GATTS_CONF_EVT:/*!< When receive confirm, the event comes */
            Log::Info(TAG, "Confirmation event.");
            break;
        case ESP_GATTS_UNREG_EVT:/*!< When unregister application id, the event comes */
            Log::Info(TAG, "Unregister event.");
            break;
        case ESP_GATTS_CREATE_EVT: /*!< When service creation is complete, the event comes */
        {
            Log::Info(TAG, "Service Created, Adding Characteristic...");

            if (service_handle != 0) {
                Log::Warn(TAG, "Service handle already set. It is getting replaced???");
            }
            service_handle = param->create.service_handle;

            if (service_handle == 0) {
                Log::Error(TAG, "Invalid service handle. Something went wrong.");
                return;
            }

            // Characteristic properties
            esp_gatt_char_prop_t char_property = 
                ESP_GATT_CHAR_PROP_BIT_READ | 
                ESP_GATT_CHAR_PROP_BIT_WRITE | 
                ESP_GATT_CHAR_PROP_BIT_NOTIFY | 
                ESP_GATT_CHAR_PROP_BIT_INDICATE;

            // ✅ Correct characteristic value (instead of using char_descr_value)
            esp_attr_value_t char_value = {
                .attr_max_len = 20,   // Maximum length
                .attr_len = 6,        // Initial value length
                .attr_value = (uint8_t*)"Hello!" // ✅ Example initial characteristic value
            };

            // ✅ Characteristic UUID (Same as before)
            esp_bt_uuid_t char_uuid = {
                .len = ESP_UUID_LEN_128,
                .uuid = {.uuid128 = {0xBB, 0xC1, 0x65, 0x23, 0x6C, 0x46, 0x51, 0xB8, 
                                    0x46, 0x49, 0x50, 0x0A, 0xDA, 0x4E, 0xE1, 0x7E}}
            };

            // ✅ Add the characteristic (Added `ESP_GATT_PERM_WRITE`)
            esp_err_t ret = esp_ble_gatts_add_char(
                service_handle,
                &char_uuid,
                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,  // ✅ Added WRITE permission
                char_property, 
                &char_value, 
                NULL
            );

            if (ret != ESP_OK) {
                Log::Error(TAG, "Failed to add characteristic: %s", esp_err_to_name(ret));
                Aborter::run();
                return;
            }

            Log::Info(TAG, "Added the characteristic!");
            Log::Info(TAG, "Service handle: %d", service_handle);
        }
        break;
        case ESP_GATTS_ADD_INCL_SRVC_EVT:/*!< When add included service complete, the event comes */
            Log::Info(TAG, "Included Service Added.");
            break;
        case ESP_GATTS_ADD_CHAR_EVT:/*!< When add characteristic complete, the event comes */
            {
                uint16_t char_handle= param->add_char.attr_handle;
    
                Log::Info(TAG, "Characteristic add, status %d, attr_handle %d, service_handle %d",
                param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
                if (char_handle == 0) {
                    ESP_LOGE(TAG, "Invalid characteristic handle. Something went wrong.");
                }
                Log::Info(TAG, "Characteristic Added.");
                
                ret = esp_ble_gatts_start_service(service_handle);
                if (ret != ESP_OK) {
                    Log::Error(TAG, "Failed to start service: %s", esp_err_to_name(ret));
                    Aborter::run();
                    // Handle the error appropriately
                    return;
                }
               
            }
            break;
        case ESP_GATTS_ADD_CHAR_DESCR_EVT:/*!< When add descriptor complete, the event comes */
            {
            }
            break;
        case ESP_GATTS_DELETE_EVT:/*!< When delete service complete, the event comes */
            Log::Info(TAG, "Service Deleted.");
            break;
        case ESP_GATTS_START_EVT:/*!< When start service complete, the event comes */
            Log::Info(TAG, "Service Started.");
            break;
        case ESP_GATTS_STOP_EVT:/*!< When stop service complete, the event comes */
            Log::Info(TAG, "Service Stopped.");
            break;
        case ESP_GATTS_CONNECT_EVT:/*!< When gatt client connect, the event comes */
            Log::Info(TAG, "Device connected.");
            break;
        case ESP_GATTS_DISCONNECT_EVT:/*!< When gatt client disconnect, the event comes */
            Log::Info(TAG, "Device disconnected. Restarting advertising...");
            startAdvertising(); // Restart advertising
            break;
        case ESP_GATTS_OPEN_EVT:/*!< When connect to peer, the event comes */
            Log::Info(TAG, "Connected to peer.");
            break;
        case ESP_GATTS_CANCEL_OPEN_EVT:/*!< When disconnect from peer, the event comes */
            Log::Info(TAG, "Disconnected from peer.");
            break;
        case ESP_GATTS_CLOSE_EVT:/*!< When gatt server close, the event comes */
            Log::Info(TAG, "GATT Server Closed.");
            break;
        case ESP_GATTS_LISTEN_EVT:/*!< When gatt listen to be connected the event comes */
            Log::Info(TAG, "GATT Listening.");
            break;
        case ESP_GATTS_CONGEST_EVT:/*!< When congest happen, the event comes */
            Log::Info(TAG, "GATT Congestion.");
            break;
        /* following is extra event */
        case ESP_GATTS_RESPONSE_EVT:/*!< When gatt send response complete, the event comes */
            Log::Info(TAG, "GATT Response.");
            break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:/*!< When gatt create table complete, the event comes */
            Log::Info(TAG, "GATT Attribute Table Created.");
            break;
        case ESP_GATTS_SET_ATTR_VAL_EVT:
            Log::Info(TAG, "GATT Attribute Value Set.");
            break;
        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
            Log::Info(TAG, "GATT Service Change Sent.");
            break;
        default:
            Log::Debug(TAG, "Unhandled GATT event: %d", event);
            break;
    }
    _feedWatchdog();
}
void Bluetooth_BLE::log_adv_data(const esp_ble_adv_data_t* adv_data) {
    if (!adv_data) {
        Log::Error(TAG, "adv_data is NULL");
        return;
    }
    Log::Info(TAG, "Adv Data:");
    Log::Info(TAG, "  set_scan_rsp: %d, include_name: %d, include_txpower: %d", 
        adv_data->set_scan_rsp, adv_data->include_name, adv_data->include_txpower);
    Log::Info(TAG, "  min_interval: %d, max_interval: %d, appearance: %d", 
        adv_data->min_interval, adv_data->max_interval, adv_data->appearance);
    Log::Info(TAG, "  manufacturer_len: %d, service_data_len: %d, service_uuid_len: %d", 
        adv_data->manufacturer_len, adv_data->service_data_len, adv_data->service_uuid_len);
    
    if (adv_data->manufacturer_len > 0 && adv_data->p_manufacturer_data) {
        ESP_LOG_BUFFER_HEX(TAG, adv_data->p_manufacturer_data, adv_data->manufacturer_len);
    }
    if (adv_data->service_data_len > 0 && adv_data->p_service_data) {
        ESP_LOG_BUFFER_HEX(TAG, adv_data->p_service_data, adv_data->service_data_len);
    }
    if (adv_data->service_uuid_len > 0 && adv_data->p_service_uuid) {
        ESP_LOG_BUFFER_HEX(TAG, adv_data->p_service_uuid, adv_data->service_uuid_len);
    }
    Log::Info(TAG, "  flag: 0x%02X", adv_data->flag);
}
void Bluetooth_BLE::log_adv_params(const esp_ble_adv_params_t* adv_params) {
    if (!adv_params) {
        Log::Error(TAG, "adv_params is NULL");
        return;
    }
    Log::Info(TAG, "Adv Params:");
    Log::Info(TAG, "  adv_int_min: %d, adv_int_max: %d, adv_type: %d", 
        adv_params->adv_int_min, adv_params->adv_int_max, adv_params->adv_type);
    Log::Info(TAG, "  own_addr_type: %d, peer_addr_type: %d, channel_map: %d", 
        adv_params->own_addr_type, adv_params->peer_addr_type, adv_params->channel_map);
    Log::Info(TAG, "  adv_filter_policy: %d", adv_params->adv_filter_policy);
}
void Bluetooth_BLE::log_service_table(const esp_gatts_attr_db_t* service_table) {
    if (!service_table) {
        Log::Error(TAG, "service_table is NULL");
        return;
    }
    Log::Info(TAG, "Service Table:");
    for (int i = 0; i < 2; ++i) {
        Log::Info(TAG, "  Attribute %d:", i);
        Log::Info(TAG, "    attr_control: %d", 
            service_table[i].attr_control);
        Log::Info(TAG, "    attr_value: {len: %d, max_len: %d, value: %p}", 
            service_table[i].att_desc.length, service_table[i].att_desc.max_length, service_table[i].att_desc.value);
    }
}
void Bluetooth_BLE::handle_read_event(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    // Example JSON data to send
    return;
    const char* json_data = "{\"status\":\"ok\"}";
    esp_gatt_rsp_t rsp;
    memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
    rsp.attr_value.handle = param->read.handle;
    rsp.attr_value.len = strlen(json_data);
    memcpy(rsp.attr_value.value, json_data, rsp.attr_value.len);
    esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
}

void Bluetooth_BLE::handle_write_event(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    // Process the received JSON data
    std::string received_data((char*)param->write.value, param->write.len);
    ESP_LOGI(TAG, "Received JSON data: %s", received_data.c_str());

    // Send a response if needed
    esp_gatt_rsp_t rsp;
    memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
    rsp.attr_value.handle = param->write.handle;
    rsp.attr_value.len = 0; // No data to send back
    esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, &rsp);
}
void Bluetooth_BLE:: startAdvertising(){
    esp_err_t err;
        if ((err = esp_ble_gap_start_advertising(adv_params)) != ESP_OK) {
        Log::Error(TAG, "Advertising failed to start: %s", esp_err_to_name(err));
        }
}