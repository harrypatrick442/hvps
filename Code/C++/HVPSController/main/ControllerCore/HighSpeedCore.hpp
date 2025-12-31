#ifndef HIGH_SPEED_CORE_HPP
#define HIGH_SPEED_CORE_HPP
#include "Enums/SystemState.hpp"
#include "Core/Event.hpp"
#include "Core/SingletonBase.hpp"
#include "SystemChecksResult.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "../Generated/HVPSConfiguration.hpp"
#include "LiveDataCache.hpp"
#include "Timing/FrequencyMeter.hpp"
class HighSpeedCore final : public SingletonBase<HighSpeedCore>{
public:
	static const char* getTag();
    Event<SystemState> onSystemStateChanged;
    Event<std::string> onError;
    Event<std::string> onMessage;
	void start();
	void stop();
	std::shared_ptr<SystemChecksResult>  runSystemChecksOnly();
	void shutDown();
	void setInError(bool value);
	SystemState getActualSystemState();
private:
    friend class SingletonBase<HighSpeedCore>;
	const float SAFE_OUTPUT_VOLTAGE = 20.0f;
	
	HighSpeedCore(
		const HVPSConfiguration& hvpsConfiguration1,
		const HVPSConfiguration& hvpsConfiguration2,
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
		Port_OutputVoltageFeedback& portOutputVoltageFeedback,
		LiveDataCache& liveDataCache,
		bool inError
	)noexcept;
	const HVPSConfiguration& _hvpsConfiguration1;
	const HVPSConfiguration& _hvpsConfiguration2;
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
	FrequencyMeter _frequencyMeter;
	
	void startCoreTask();
	void startFrequencyMeasurement();
	void _run();
	std::shared_ptr<SystemChecksResult> doSystemChecks();
	void doIdle();
	void doLive();
	void doShutDown();
	void doRunningSystemChecks();
	void doError();
	SystemState getDesiredSystemState();
	void setDesiredSystemState(SystemState systemState);
	void setActualSystemState(SystemState systemState);
	bool isShuttingDownOrShutDown();
	bool getInError();
	void dispatchSystemStateChanged(SystemState systemState);
	void dispatchError(std::string errorMessage);
	void dispatchMessage(std::string message);
	void loopFrequencyMeasurement();
};

#endif // HIGH_SPEED_CORE_HPP
