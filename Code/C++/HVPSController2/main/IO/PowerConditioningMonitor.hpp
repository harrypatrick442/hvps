#pragma once
#include "Core/SingletonBase.hpp"
#include "../Generated/HVPSConfiguration.hpp"
#include "../Generated/HVPSConfiguration.hpp"
#include "ADC/IADCSession.hpp"
class PowerConditioningMonitor:public SingletonBase<PowerConditioningMonitor>{
    friend class SingletonBase<PowerConditioningMonitor>;
	private:
		static inline constexpr int SAMPLE_INTERVAL_MS = 10;
		static inline constexpr int WINDOW_SAMPLES     = 100;
		static inline constexpr int RELAY_SWITCH_TIME_MILLISECONDS = 1000;
		static inline constexpr int TALK_INTERVAL_MILLISECONDS = 2000;
		static inline constexpr int READY_VOLTAGE_HYSTERESIS_WINDOW_UPPER_BOUND = 29;//TODO
		static inline constexpr int READY_VOLTAGE_HYSTERESIS_WINDOW_LOWER_BOUND = 28;
		
		const HVPSConfiguration& _config1;
		const HVPSConfiguration& _config2;
		size_t _index;
		uint64_t _talkNext;
		std::atomic<bool> _ready;
		float _voltages[WINDOW_SAMPLES];
	protected:
		PowerConditioningMonitor(const HVPSConfiguration& config1, const HVPSConfiguration& config2);
	public:
		static const char* getTag();
		bool getIsReady();
		void waitForSoftStart();
	private:
		void run();
		void updateReady(bool value);
		bool canTalk();
		const char* tellWhatWaitingOn(bool reachedMinimumVoltage, bool voltageStoppedIncreasing);
		float computeLatestAverageVoltage(IADCSession&& adc);
};