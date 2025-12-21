#pragma once
#include <atomic>
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Core/FloatAndTime.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "Core/Event.hpp"
#include <atomic>
class LiveDataCache final: public SingletonBase<LiveDataCache>{
public:

	static const char* getTag();
	
	DISALLOW_COPY_MOVE(LiveDataCache);
    // Output Voltage (V)
    FloatAndTime getOutputVoltage()const noexcept;

    // Output Current (A)
    FloatAndTime getOutputCurrent()const noexcept;

    // Total Output Energy (J)
    FloatAndTime getTotalOutputEnergy()const noexcept;
    void   setTotalOutputEnergy(float v) noexcept;

    // First Stage Voltage (V)
    FloatAndTime getFirstStageVoltage()const noexcept;
    void   setFirstStageVoltage(float v) noexcept;

    // Peak Primary Current (A)
    FloatAndTime getPeakPrimaryCurrent()const noexcept;
    void   setPeakPrimaryCurrent(float v) noexcept;

private:
    // Let the base construct us
    friend class SingletonBase<LiveDataCache>;
	LiveDataCache(
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
		Port_OutputVoltageFeedback& portOutputVoltageFeedback
	) noexcept;
	
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	
    std::atomic<FloatAndTime> _outputVoltage{FloatAndTime{0.0, 0}};
    std::atomic<FloatAndTime> _outputCurrent{FloatAndTime{0.0, 0}};
    std::atomic<FloatAndTime> _totalOutputEnergy{FloatAndTime{0.0, 0}};
    std::atomic<FloatAndTime> _firstStageVoltage{FloatAndTime{0.0, 0}};
    std::atomic<FloatAndTime> _peakPrimaryCurrent{FloatAndTime{0.0, 0}};
	EventConnection _eventConnectionGotOutputVoltage;
	EventConnection _eventConnectionGotFirstStageVoltage;
	
    void   setOutputVoltage(float v) noexcept;
    void   setOutputCurrent(float v) noexcept;
	void handleGotOutputVoltage(float voltage) noexcept;
	void handleGotFirstStageVoltage(float voltage) noexcept;
};

