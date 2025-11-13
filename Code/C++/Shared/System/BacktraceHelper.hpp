#pragma once
#include <cstdint>
#include <cstddef>

class BacktraceHelper{
	public:
		static size_t getBacktrace(uint32_t* out_pcs, uint32_t* out_sps, size_t max_depth);
};