#include "CPUClockFrequencyHelper.hpp"
#include "esp_private/esp_clk_tree_common.h" // For esp_clk_tree_src_get_freq_hz()
#include "soc/clk_tree_defs.h"    // For SOC_CLK_SRC_CPU and precision enums
uint32_t CPUClockFrequencyHelper::getClockFrequencyHzExact(){
	uint32_t cpu_freq_hz;
	esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU, ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, &cpu_freq_hz);
	return cpu_freq_hz;
}
uint32_t CPUClockFrequencyHelper::getClockFrequencyHzApproximate(){
	uint32_t cpu_freq_hz;
	esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &cpu_freq_hz);
	return cpu_freq_hz;
}
float CPUClockFrequencyHelper::getClockFrequencyMHZExact(){
	uint32_t hz = getClockFrequencyHzExact();
	return static_cast<float>(hz)/1000000.0f;
}
float CPUClockFrequencyHelper::getClockFrequencyMHZApproximate(){
	uint32_t hz = getClockFrequencyHzApproximate();
	return static_cast<float>(hz)/1000000.0f;
}