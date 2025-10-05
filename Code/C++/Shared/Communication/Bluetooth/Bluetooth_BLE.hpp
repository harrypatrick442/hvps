#ifndef BLUETOOTH_BLE_HPP
#define BLUETOOTH_BLE_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_system.h"
#include "esp_spp_api.h"
#include "../../System/WatchdogFeeder.hpp"
#include <array>
#include <string>

#define DEVICE_NAME "ESP32_BLE"
#define CHARACTERISTIC_UUID 0xFF01

class Bluetooth_BLE{
public:
    // Modify initialize method to accept UUIDs
    static void initialize(
    const char* deviceName, 
    const char* serverName,
    std::array<uint8_t, ESP_UUID_LEN_128>& serviceUUID,
    uint16_t characteristicUUID,
    DelegateFeedWatchdog& feedWatchdog);

    static Bluetooth_BLE& getInstance();

private:
    Bluetooth_BLE(
    const char* deviceName, 
    const char* serverName,
    std::array<uint8_t, ESP_UUID_LEN_128>& serviceUUID,
    uint16_t characteristicUUID,
    DelegateFeedWatchdog& feedWatchdog);

    static Bluetooth_BLE* _instance;
    static const char* TAG;
    const char* _deviceName;
    const char* _serverName;

    std::array<uint8_t, ESP_UUID_LEN_128>& _serviceUUID;
    uint16_t _characteristicUUID;
    DelegateFeedWatchdog& _feedWatchdog;
    esp_bt_uuid_t esp_service_uuid;
    esp_bt_uuid_t esp_char_uuid;
    esp_gatts_attr_db_t* service_table;
    esp_ble_adv_params_t* adv_params;
    esp_ble_adv_data_t* adv_data;
    std::string characteristic_value;
    uint16_t service_handle;
    static void checkInitialized();
    static void gap_event_handler_static(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    static void gatt_event_handler_static(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
    static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
    void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    void gatt_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
    static void log_adv_data(const esp_ble_adv_data_t* adv_data);
    static void log_adv_params(const esp_ble_adv_params_t* adv_params);
    static void log_service_table(const esp_gatts_attr_db_t* service_table);

    void handle_read_event(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
    void handle_write_event(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
    void startAdvertising();
};

#endif // BLUETOOTH_BLE_HPP