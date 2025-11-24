#ifndef SYSTEMSTATE_HPP
#define SYSTEMSTATE_HPP

// Enum for System State
enum SystemState {
	Unknown = 0,
	Idle = 1,
	Live = 2,
	RunningSystemChecks = 3,
	ShuttingDown = 4,
	ShutDown = 5,
	Error = 6
};
#endif