#pragma once
#include "esp_log.h"
#include "soc/rmt_struct.h"
#include "driver/periph_ctrl.h"

class RMTHelper {
public:

    static inline void forceHardwareReset()
    {
        periph_module_disable(PERIPH_RMT_MODULE);
        periph_module_reset(PERIPH_RMT_MODULE);
        periph_module_enable(PERIPH_RMT_MODULE);
    }

    static inline void dumpRMTState(const char* tag = "RMT_DIAG")
    {
        ESP_LOGW(tag, "------ ESP32 RMT Register Dump (IDF 5.4) ------");

        for (int ch = 0; ch < 8; ch++)
        {
            const auto& c0 = RMT.conf_ch[ch].conf0;
            const auto& c1 = RMT.conf_ch[ch].conf1;

            bool tx = c1.tx_start || c1.tx_conti_mode;
            bool rx = c1.rx_en;

            ESP_LOGW(tag,
                "CH%d: TX=%d RX=%d mem_owner=%d mem_size=%u div=%u idle_out=%d idle_lv=%d",
                ch,
                (int)tx,
                (int)rx,
                (int)c1.mem_owner,
                (unsigned)c0.mem_size,
                (unsigned)c0.div_cnt,
                (int)c1.idle_out_en,
                (int)c1.idle_out_lv
            );
        }

        ESP_LOGW(tag, "INT_ST=0x%08X", (unsigned)RMT.int_st.val);
        ESP_LOGW(tag, "------ END DUMP ------");
    }

    static inline void analyseRMTUsage(const char* tag = "RMT_ANALYSIS")
    {
        int used = 0, tx = 0, rx = 0;

        for (int ch = 0; ch < 8; ch++)
        {
            auto& c1 = RMT.conf_ch[ch].conf1;

            bool is_tx = c1.tx_start || c1.tx_conti_mode;
            bool is_rx = c1.rx_en;

            if (is_tx || is_rx)
            {
                used++;
                if (is_tx) tx++;
                if (is_rx) rx++;
            }
        }

        ESP_LOGW(tag, "Used channels: %d/8   TX=%d   RX=%d", used, tx, rx);

        if (rx > 0)
            ESP_LOGW(tag, "RX channels active BEFORE initialization — very likely Bluetooth or WiFi/BT coexistence.");

        if (used == 0)
            ESP_LOGW(tag, "No active channels — if allocation still fails, candidate slots may be reserved internally by BT.");
    }
	static inline void breakpoint_on_rmt_touch(const char* msg) {
		ESP_EARLY_LOGE("RMTTOUCH", "RMT touched: %s", msg);
		esp_backtrace_print(20);
	}
};
