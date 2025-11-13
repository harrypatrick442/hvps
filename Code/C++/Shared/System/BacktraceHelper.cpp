#include "BacktraceHelper.hpp"
#include "esp_debug_helpers.h"

#include "esp_cpu_utils.h"

size_t BacktraceHelper::getBacktrace(
	uint32_t* out_pcs, uint32_t* out_sps, size_t max_depth)
{
	if (!out_pcs || max_depth <= 0) return 0;

	// 1. Get first stack frame
	esp_backtrace_frame_t stk_frame = {};
	esp_backtrace_get_start(&stk_frame.pc, &stk_frame.sp, &stk_frame.next_pc);

	size_t i = 0;
	bool corrupted = false;

	// 2. Process first frame
	out_pcs[i] = esp_cpu_process_stack_pc(stk_frame.pc);
	if (out_sps) out_sps[i] = stk_frame.sp;
	++i;

	// 3. Traverse remaining frames
	while (i < max_depth && stk_frame.next_pc != 0 && !corrupted) {
		if (!esp_backtrace_get_next_frame(&stk_frame)) {
			corrupted = true;
			break;
		}

		out_pcs[i] = esp_cpu_process_stack_pc(stk_frame.pc);
		if (out_sps) out_sps[i] = stk_frame.sp;
		++i;
	}

	return i;  // number of frames collected
}