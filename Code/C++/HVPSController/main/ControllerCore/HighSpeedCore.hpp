#ifndef HIGH_SPEED_CORE_HPP
#define HIGH_SPEED_CORE_HPP
#include "../Enums/SystemState.hpp"
#include "../Core/Event.hpp"
#include "../Core/SingletonBase.hpp"
class HighSpeedCore final : public SingletonBase<HighSpeedCore>{
public:
    static inline constexpr const char* TAG = "HighSpeedCore";
    Event<SystemState> onSystemStateChanged;
	SimpleResult start();
	SimpleResult stop();
	SimpleResult runSystemChecksOnly();
	SimpleResult shutDown();
private:
    friend class SingletonBase<HighSpeedCore>;
	
	const uint64_t ON_TIME_US = 62;
	const uint64_t ON_TIME_US_2 = 62;
	const uint64_t OFF_TIME_US = 62;
	const uint64_t OFF_TIME_US_2 = 62;
	const double SAFE_OUTPUT_VOLTAGE = 30;
	
	HighSpeedCore(
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
		Port_OutputVoltageFeedback& portOutputVoltageFeedback
	)noexcept;
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	std::atomic<bool> _shuttingOrShutDown;
	std::atomic<SystemState> _systemState;
	std::atomic<SystemState> _desiredSystemState;
	std::atomic<bool> _shuttingOrShutDown_2;
	std::mutex _mutexControlInput;
	
	std::mutex _mutexSystemChecksResult;
	shared_ptr<SystemChecksResult> _systemChecksResult;
	Latch _runSystemChecksLatch;
	void startCoreTask()
	void _run();
	void doSystemChecks();
	void doIdle();
	void doLive();
	void doShutdown();
	void dispatchSystemStateChanged(SystemState systemState);
};

#endif // HIGH_SPEED_CORE_HPP
