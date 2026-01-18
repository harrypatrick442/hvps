#ifndef HIGH_SPEED_CORE_HPP
#define HIGH_SPEED_CORE_HPP
#include "../Enums/SystemState.hpp"
#include "Core/Event.hpp"
#include "Core/SingletonBase.hpp"
#include "SystemChecksResult.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "LiveDataCache.hpp"
#include <optional>
class HighSpeedCore final : public SingletonBase<HighSpeedCore>{
public:
	static const char* getTag();
    Event<SystemState> onSystemStateChanged;
    Event<std::string> onError;
	void start();
	void stop();
	std::shared_ptr<SystemChecksResult>  runSystemChecksOnly();
	void shutDown();
private:
    friend class SingletonBase<HighSpeedCore>;
	const float SAFE_OUTPUT_VOLTAGE = 30;
	
	HighSpeedCore(
		HVPSConfiguration& config1,
		HVPSConfiguration& config2,
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
		Port_OutputVoltageFeedback& portOutputVoltageFeedback,
		LiveDataCache& liveDataCache
	)noexcept;
	HVPSConfiguration& _config1;
	HVPSConfiguration& _config2;
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	LiveDataCache& _liveDataCache;
	std::atomic<bool> _shuttingOrShutDown;
	std::atomic<SystemState> _actualSystemState;
	std::atomic<SystemState> _desiredSystemState;
	std::atomic<bool> _shuttingOrShutDown_2;
	std::mutex _mutexControlInput;
	
	std::mutex _mutexSystemChecksResult;
	std::shared_ptr<SystemChecksResult> _systemChecksResult;
	Latch _runSystemChecksLatch;
	volatile bool _watchdogFed;
	volatile bool _watchdogFail;
	
	void startCoreTask();
	static void _run_taskTrampoline(void* arg);
	void _run();
	std::shared_ptr<SystemChecksResult> doSystemChecks();
	void doIdle();
	void doLive();
	void doShutDown();
	SystemState getDesiredSystemState();
	SystemState getActualSystemState();
	void setDesiredSystemState(SystemState systemState);
	void setActualSystemState(SystemState systemState);
	bool isShuttingDownOrShutDown();
	void dispatchSystemStateChanged(SystemState systemState);
	void dispatchError(std::string errorMessage);
	std::optional<InterruptTimer> initializeLiveWatchdog();
	inline void IRAM_ATTR HighSpeedCore::feedWatchdog();
	inline void IRAM_ATTR HighSpeedCore::checkWatchdog();
	bool IRAM_ATTR HighSpeedCore::liveWatchdogTimerTrampoline(void *arg);
};

#endif // HIGH_SPEED_CORE_HPP
