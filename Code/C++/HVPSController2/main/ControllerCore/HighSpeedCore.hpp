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
#include "Enums/ValueBoundType.hpp"
#include "Timing/InterruptTimer.hpp"
#include <optional>
class HighSpeedCore final : public SingletonBase<HighSpeedCore>{
private:
	inline static constexpr uint32_t MIN_CYCLES_FOR_EXACT = 2000;
	inline static constexpr uint64_t MIN_DT_US_FOR_EXACT = 2000;
	/*If your frequency is f Hz and you want 0.05% error:
	Number of cycles N ≥ 1 / relative_error
	N ≥ 1 / 0.0005 = 2000 cycles
	ensure dtUs >= 2 ms for time precision to not dominate.
	TODO double check this but not high priority due to how 
	frequency is intended to be used (just displayed in UI for peace of mind)*/
	
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
	std::atomic<bool> _shuttingOrShutDown_2;
	std::atomic<SystemState> _actualSystemState;
	std::atomic<SystemState> _desiredSystemState;
	std::atomic<bool> _inError;
	std::mutex _mutexControlInput;
	std::mutex _mutexSystemChecksResult;
	std::shared_ptr<SystemChecksResult> _systemChecksResult;
	Latch _runSystemChecksLatch;
	volatile uint64_t _startLiveTimeUs;
	volatile uint64_t _nCyclesCount;
	volatile uint16_t _peakCurrentSenseVoltageRaw;
	volatile bool _watchdogFed;
	volatile bool _watchdogFail;
	
public:
    Event<SystemState> onSystemStateChanged;
    Event<std::string> onError;
    Event<std::string> onMessage;
    Event<std::string> onWarning;

public:
	static const char* getTag();
	void start();
	void stop();
	std::shared_ptr<SystemChecksResult>  runSystemChecksOnly();
	void shutDown();
	void setInError(bool value);
	SystemState getActualSystemState();
	float getFrequencyHz(ValueBoundType& valueBoundType);
	float getPeakPrimaryCurrent(ValueBoundType& valueBoundType);
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
	void dispatchWarning(std::string message);
	void loopFrequencyMeasurement();
	void calculateAdditionalShutdownTime(float voltage, float& timeSeconds, float& time2Seconds);
	std::shared_ptr<InterruptTimer> initializeLiveWatchdog();
	inline void IRAM_ATTR feedWatchdog();
	inline void IRAM_ATTR checkWatchdog();
	static bool IRAM_ATTR liveWatchdogTimerTrampoline(void *arg);
	bool testLiveWatchdog(std::shared_ptr<InterruptTimer> liveWatchdog);
};

#endif // HIGH_SPEED_CORE_HPP
