#ifndef BLUETOOTH_BLE_HPP
#define BLUETOOTH_BLE_HPP

#include "esp_gatts_api.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_log.h"
#include <cstring>

class Bluetooth_BLE {
public:
    static void initialize();  // Call this from main()
    static void startAdvertising();
    
private:
    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    static void gatt_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
    
    static constexpr uint16_t SERVICE_UUID = 0x180F;  // 16-bit UUID for service
    static constexpr uint16_t CHAR_UUID = 0x2A19;     // 16-bit UUID for characteristic
    static constexpr int MAX_GATT_ATTRS = 10;         // Maximum number of GATT attributes

    static esp_gatt_if_t gatt_interface;
    static uint16_t service_handle;
    static uint16_t char_handle;
};

#endif // BLUETOOTH_BLE_HPP

#include "Bluetooth_BLE.hpp"
#include "esp_bt.h"
#include "../../Storage/Flash.hpp"

static const char *TAG = "Bluetooth_BLE";

esp_gatt_if_t Bluetooth_BLE::gatt_interface = 0;
uint16_t Bluetooth_BLE::service_handle = 0;
uint16_t Bluetooth_BLE::char_handle = 0;

void Bluetooth_BLE::initialize() {
    ESP_LOGI(TAG, "Initializing Bluetooth BLE...");

    Flash::initialize();
    esp_err_t ret;

    // Initialize Bluetooth Controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "BT Controller Init Failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "BT Controller Enable Failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid Init Failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid Enable Failed: %s", esp_err_to_name(ret));
        return;
    }

    // Set Bluetooth device name
    ret = esp_ble_gap_set_device_name("HVPSController2");
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Set Device Name Failed: %s", esp_err_to_name(ret));
        return;
    }

    // Register GAP and GATT event handlers
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_register_callback(gatt_event_handler);
    esp_ble_gatts_app_register(0);

    ESP_LOGI(TAG, "Bluetooth Initialized and ready!");
}

void Bluetooth_BLE::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT: {
            ESP_LOGI(TAG, "Advertisement Data Set. Starting Advertising...");
            startAdvertising();
            break;
        }
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT: {
            ESP_LOGI(TAG, "Advertising started successfully.");
            break;
        }
        default: {
            ESP_LOGD(TAG, "Unhandled GAP event: %d", event);
            break;
        }
    }
}

void Bluetooth_BLE::gatt_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_REG_EVT: {
            ESP_LOGI(TAG, "GATT Server Registered.");
            gatt_interface = gatts_if;

            esp_gatt_srvc_id_t service_id = {
                .id = {.uuid = {.len = ESP_UUID_LEN_16, .uuid = {.uuid16 = SERVICE_UUID}}},
                .is_primary = true
            };

            esp_ble_gatts_create_service(gatt_interface, &service_id, MAX_GATT_ATTRS);
            break;
        }
        case ESP_GATTS_CREATE_EVT: {
            ESP_LOGI(TAG, "Service Created. Handle: %d", param->create.service_handle);
            service_handle = param->create.service_handle;

            esp_bt_uuid_t char_uuid = {
                .len = ESP_UUID_LEN_16,
                .uuid = {.uuid16 = CHAR_UUID}
            };

            esp_attr_value_t attr_value = {
                .attr_max_len = 512,
                .attr_len = 13,
                .attr_value = (uint8_t*)"Hello, World!"
            };

            esp_gatt_char_prop_t char_property = 
                ESP_GATT_CHAR_PROP_BIT_READ | 
                ESP_GATT_CHAR_PROP_BIT_WRITE | 
                ESP_GATT_CHAR_PROP_BIT_NOTIFY;

            esp_err_t ret = esp_ble_gatts_add_char(service_handle, &char_uuid,
                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, char_property, &attr_value, NULL);

            if (ret == ESP_OK) {
                ESP_LOGI(TAG, "Characteristic Added Successfully.");
            } else {
                ESP_LOGE(TAG, "Failed to Add Characteristic: %s", esp_err_to_name(ret));
            }
            break;
        }
        case ESP_GATTS_ADD_CHAR_EVT: {
            ESP_LOGI(TAG, "Characteristic Added. Handle: %d", param->add_char.attr_handle);
            char_handle = param->add_char.attr_handle;
            
            // Add Characteristic Description (0x2901)
            esp_bt_uuid_t descr_uuid = {
                .len = ESP_UUID_LEN_16,
                .uuid = {.uuid16 = ESP_GATT_UUID_CHAR_DESCRIPTION},
            };

            esp_attr_value_t descr_value = {
                .attr_max_len = 20,
                .attr_len = 8,
                .attr_value = (uint8_t*)"My Char"
            };

            esp_ble_gatts_add_char_descr(
                service_handle, 
                &descr_uuid, 
                ESP_GATT_PERM_READ, 
                &descr_value, 
                NULL
            );

            esp_ble_gatts_start_service(service_handle);
            break;
        }
        case ESP_GATTS_CONNECT_EVT: {
            ESP_LOGI(TAG, "Device connected.");
            break;
        }
        case ESP_GATTS_DISCONNECT_EVT: {
            ESP_LOGI(TAG, "Device disconnected. Restarting advertising...");
            startAdvertising();
            break;
        }
        default: {
            ESP_LOGD(TAG, "Unhandled GATT event: %d", event);
            break;
        }
    }
}

void Bluetooth_BLE::startAdvertising() {  uint16_t service_uuid = SERVICE_UUID;  // Ensure it's properly formatted

    esp_ble_adv_data_t adv_data = {
        .set_scan_rsp = false,
        .include_name = true,
        .include_txpower = true,
        .min_interval = 0x20,
        .max_interval = 0x40,
        .appearance = 0x00,
        .manufacturer_len = 0,
        .p_manufacturer_data = NULL,
        .service_data_len = 0,
        .p_service_data = NULL,
        .service_uuid_len = sizeof(service_uuid),  // Corrected UUID size
        .p_service_uuid = (uint8_t *)&service_uuid,  // Pass valid pointer
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
    };

    esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Advertising Data Config Failed: %s", esp_err_to_name(ret));
        return;
    }

    esp_ble_adv_params_t adv_params = {
        .adv_int_min = 0x20,
        .adv_int_max = 0x40,
        .adv_type = ADV_TYPE_IND,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .channel_map = ADV_CHNL_ALL,
        .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
    };

    esp_ble_gap_start_advertising(&adv_params);
}
