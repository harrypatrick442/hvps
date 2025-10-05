#include "WatchdogFeeder.hpp"
#include "Aborter.hpp"
#include "esp_task_wdt.h"  // Ensure ESP-IDF is correctly installed and the include path is set
#include "esp_system.h"    // Include the header for esp_random#include <inttypes.h>
#include "driver/timer.h"  // Include the header for timer group definitions
#include <stdio.h>
#include "../Logging/Log.hpp"
void disable_all_watchdogs() {
    //esp_task_wdt_deinit(); 
    // Disable Task Watchdog Timer (TWDT)
    //ESP_ERROR_CHECK(esp_task_wdt_deinit());
/*
    // Disable Timer Group 0 Watchdog Timer
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // Disable write protection
    TIMERG0.wdt_config0.en = 0; // Disable the watchdog
    TIMERG0.wdt_wprotect = 0; // Enable write protection

    // Disable Timer Group 1 Watchdog Timer
    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // Disable write protection
    TIMERG1.wdt_config0.en = 0; // Disable the watchdog
    TIMERG1.wdt_wprotect = 0; // Enable write protection

    // Disable RTC Watchdog Timer
    WRITE_PERI_REG(RTC_CNTL_WDTWPROTECT_REG, RTC_CNTL_WDT_WKEY_VALUE); // Disable write protection
    CLEAR_PERI_REG_MASK(RTC_CNTL_WDTCONFIG0_REG, RTC_CNTL_WDT_EN); // Disable the watchdog
    WRITE_PERI_REG(RTC_CNTL_WDTWPROTECT_REG, 0); // Enable write protection*/
}
WatchdogFeeder* WatchdogFeeder::_instance = nullptr;
const char* WatchdogFeeder::TAG = "WatchdogFeeder";
// Define moving objects for Mr. Watchdog's *special* diet
WatchdogFeeder& WatchdogFeeder::initialize(uint32_t timeout) {
    if(_instance != nullptr) {
        Aborter::safeAbort(TAG, "WatchdogFeeder already initialized");
        return *_instance;
    }
    esp_reset_reason_t reason = esp_reset_reason();
    printf("[RESET] Reset reason: %d\n", reason);

    if (reason == ESP_RST_DEEPSLEEP) {
        printf("❌ ESP woke up from deep sleep! Something is forcing it.\n");
    }
    disable_all_watchdogs();
    /*
    esp_task_wdt_config_t* config = new esp_task_wdt_config_t{
        .timeout_ms = timeout,  // Set timeout to 10 seconds
        .trigger_panic = true   
    };
    esp_task_wdt_init(config);  // Set timeout to 10 seconds
    esp_task_wdt_add(NULL);       // Attach the current task to the watchdog
    */
    _instance = new WatchdogFeeder();
    return *_instance;
}
WatchdogFeeder& WatchdogFeeder::getInstance() {
    if(_instance == nullptr) {
        Aborter::safeAbort(TAG, "WatchdogFeeder not initialized. Call initialize() first.");
        return *_instance;
    }
    return *_instance;
}
WatchdogFeeder::WatchdogFeeder() : _successfulHunts(0), _lastKillingWasIndex(0) {
    // Mr. Watchdog is always hungry
    printf("🐶 Mr. Watchdog is always hungry!\n");
    
}
const char* WatchdogFeeder::getMovingPrey() {
    static uint8_t preyIndex = 0;
    const char* prey[] = {
        "🏃‍♂️ A poor soul trying to escape",
        "🚗 A self-driving Tesla",
        "🚀 A SpaceX Falcon 9 booster",
        "🏎️ A Formula 1 car at full throttle",
        "🐔 A terrified chicken crossing the road",
        "🦘 A kangaroo mid-jump",
        "🏃‍♀️ A track athlete sprinting for gold",
        "🐿️ A squirrel zig-zagging through trees",
        "🐭 Jerry (from Tom & Jerry) - Run, Jerry, Run!",
        "🕵️ A spy trying to evade capture",
        "🤖 A rogue AI robot attempting to escape shutdown",
        "👽 A UFO hovering suspiciously low",
        "🐉 A dragon in mid-flight (Wait... where did this come from?)"
    };
    const uint8_t preyLength = sizeof(prey) / sizeof(prey[0]);
    if(preyIndex>=preyLength) {
        preyIndex = 0;
    }
    return prey[preyIndex++];
}

// 🐶 Mr. Watchdog *HUNTS* instead of just eating
void WatchdogFeeder::huntForFood() {
    return;
    esp_task_wdt_reset();
    _successfulHunts++;

    const char* prey = getMovingPrey();
    printf("🐶 Mr. Watchdog caught: %s! Total hunts: %" PRIu64 "\n", prey, _successfulHunts);

    if (_successfulHunts % 5 == 0) {
        printf("💀 Mr. Watchdog is becoming **FASTER**! NOTHING CAN ESCAPE HIM NOW!\n");
    }
}
// Implementation of the function that returns a function pointer

DelegateFeedWatchdog WatchdogFeeder::getFeedWatchdog() {
    return [this]() { this->huntForFood(); };  // ✅ Captures `this` for instance
}