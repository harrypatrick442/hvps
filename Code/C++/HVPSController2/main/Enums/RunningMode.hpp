#ifndef RUNNING_MODE_HPP
#define RUNNING_MODE_HPP

// Enum for System State
enum class RunningMode {
    Running,
    RunningSystemChecksOnly,
    Stopping,
    Stopped,
    ShuttingDown,
    ShutDown
};
#endif