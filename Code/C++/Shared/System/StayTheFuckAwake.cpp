#include "StayTheFuckAwake.hpp"
#include <cstdio>

void StayTheFuckAwake::disableSleepSources() {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    printf("❌ Disabled ALL sleep wakeup sources!\n");
}

void StayTheFuckAwake::disablePowerManagement() {
    esp_pm_config_t pm_config = {
        .max_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .light_sleep_enable = false
    };
    esp_pm_configure(&pm_config);
    printf("❌ Power management DISABLED! CPU runs at FULL SPEED!\n");
}

void StayTheFuckAwake::disableTicklessIdle() {
    vTaskSuspendAll();
    printf("❌ FreeRTOS Tickless Idle DISABLED!\n");
}

void StayTheFuckAwake::disableWiFiPowerSave() {
    esp_wifi_set_ps(WIFI_PS_NONE);
    printf("❌ Wi-Fi Power Save Mode DISABLED!\n");
}

void StayTheFuckAwake::disableBluetoothPowerSave() {
   // esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
   // printf("❌ Bluetooth Power Save Mode DISABLED!\n");
}

void StayTheFuckAwake::disableWatchdog() {
    //esp_task_wdt_deinit();
    //printf("❌ Watchdog Timer DISABLED! Nothing is watching you now. 👀\n");
}
void StayTheFuckAwake::keepRunningForever() {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("💡 ESP32 is still running... No sleep, no mercy! 💪\n");
    }
}
