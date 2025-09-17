#ifndef SYSTEMSTATE_HPP
#define SYSTEMSTATE_HPP

// Enum for System State
enum class SystemState {
    Idle,
    Live,
	RunningSystemChecks,
    ShuttingDown,
    ShutDown
};
#endif