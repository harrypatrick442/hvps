#ifndef HIGH_SPEED_CORE_HPP
#define HIGH_SPEED_CORE_HPP
#include "Enums/SystemState.hpp"
#include "Core/Event.hpp"
#include "Core/SingletonBase.hpp"
#include "SystemChecksResult.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "LiveDataCache.hpp"
class HighSpeedCore final : public SingletonBase<HighSpeedCore>{
public:
    static inline constexpr const char* TAG = "HighSpeedCore";
    Event<SystemState> onSystemStateChanged;
    Event<std::string> onError;
	void start();
	void stop();
	std::shared_ptr<SystemChecksResult>  runSystemChecksOnly();
	void shutDown();
	void setInError(bool value);
	SystemState getActualSystemState();
private:
    friend class SingletonBase<HighSpeedCore>;
	
	const uint64_t ON_TIME_US = 62;
	const uint64_t ON_TIME_US_2 = 62;
	const uint64_t OFF_TIME_US = 62;
	const uint64_t OFF_TIME_US_2 = 62;
	const float SAFE_OUTPUT_VOLTAGE = 30;
	
	HighSpeedCore(
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
		Port_OutputVoltageFeedback& portOutputVoltageFeedback,
		LiveDataCache& liveDataCache,
		bool inError
	)noexcept;
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	LiveDataCache& _liveDataCache;
	/*
	DO NOT EVER SET _shuttingOrShutDown or _shuttingOrShutDown_2 BACK TO FALSE. EVER!!!!
	A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
	THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
	REACTIVATED WITHOUT REBOOT!!
	*/
	std::atomic<bool> _shuttingOrShutDown;
	std::atomic<SystemState> _actualSystemState;
	std::atomic<SystemState> _desiredSystemState;
	std::atomic<bool> _shuttingOrShutDown_2;
	std::atomic<bool> _inError;
	std::mutex _mutexControlInput;
	
	std::mutex _mutexSystemChecksResult;
	std::shared_ptr<SystemChecksResult> _systemChecksResult;
	Latch _runSystemChecksLatch;
	void startCoreTask();
	void _run();
	std::shared_ptr<SystemChecksResult> doSystemChecks();
	void doIdle();
	void doLive();
	void doShutDown();
	void doError();
	SystemState getDesiredSystemState();
	void setDesiredSystemState(SystemState systemState);
	void setActualSystemState(SystemState systemState);
	bool isShuttingDownOrShutDown();
	bool getInError();
	void dispatchSystemStateChanged(SystemState systemState);
	void dispatchError(std::string errorMessage);
};

#endif // HIGH_SPEED_CORE_HPP
