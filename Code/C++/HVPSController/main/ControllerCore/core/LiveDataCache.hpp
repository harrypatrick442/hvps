#pragma once
#include <atomic>
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Core/DoubleAndTime.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "Core/Event.hpp"
#include <atomic>
class LiveDataCache final: public SingletonBase<LiveDataCache>{
public:
    static inline constexpr const char* TAG = "LiveDataCache";
	
	DISALLOW_COPY_MOVE(LiveDataCache);
    // Output Voltage (V)
    DoubleAndTime getOutputVoltage()const noexcept;

    // Output Current (A)
    DoubleAndTime getOutputCurrent()const noexcept;

    // Total Output Energy (J)
    DoubleAndTime getTotalOutputEnergy()const noexcept;
    void   setTotalOutputEnergy(double v) noexcept;

    // First Stage Voltage (V)
    DoubleAndTime getFirstStageVoltage()const noexcept;
    void   setFirstStageVoltage(double v) noexcept;

    // Peak Primary Current (A)
    DoubleAndTime getPeakPrimaryCurrent()const noexcept;
    void   setPeakPrimaryCurrent(double v) noexcept;

private:
    // Let the base construct us
    friend class SingletonBase<LiveDataCache>;
	LiveDataCache(
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
		Port_OutputVoltageFeedback& portOutputVoltageFeedback
	) noexcept;
	
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	
    std::atomic<DoubleAndTime> _outputVoltage{DoubleAndTime{0.0, 0}};
    std::atomic<DoubleAndTime> _outputCurrent{DoubleAndTime{0.0, 0}};
    std::atomic<DoubleAndTime> _totalOutputEnergy{DoubleAndTime{0.0, 0}};
    std::atomic<DoubleAndTime> _firstStageVoltage{DoubleAndTime{0.0, 0}};
    std::atomic<DoubleAndTime> _peakPrimaryCurrent{DoubleAndTime{0.0, 0}};
	EventConnection _eventConnectionGotOutputVoltage;
	EventConnection _eventConnectionGotFirstStageVoltage;
	
    void   setOutputVoltage(double v) noexcept;
    void   setOutputCurrent(double v) noexcept;
	void handleGotOutputVoltage(double voltage) noexcept;
	void handleGotFirstStageVoltage(double voltage) noexcept;
};

