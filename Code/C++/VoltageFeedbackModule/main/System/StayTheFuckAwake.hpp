#ifndef STAY_THE_FUCK_AWAKE_HPP
#define STAY_THE_FUCK_AWAKE_HPP

#include "esp_sleep.h"
#include "esp_pm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
//#include "esp_bt.h"
#include "esp_task_wdt.h"

class StayTheFuckAwake {
public:
    static void initialize();
    static void disableAllSleep();
    static void keepRunningForever();

private:
    static void disableSleepSources();
    static void disablePowerManagement();
    static void disableTicklessIdle();
    static void disableWiFiPowerSave();
    static void disableBluetoothPowerSave();
    static void disableWatchdog();
};

#endif // STAY_THE_FUCK_AWAKE_HPP
