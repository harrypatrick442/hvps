#ifndef OUTPUTS_LIFETIME_FLAGS_HPP
#define OUTPUTS_LIFETIME_FLAGS_HPP

enum class OutputsLifetimeFlags : uint8_t {
	None       = 0       // alias
	Initialized   = 1 << 0, // 0000 0001
	Safe          = 1 << 1, // 0000 0010
};
#endif