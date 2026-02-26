#pragma once
#include <atomic>
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Core/FloatAndTime.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "Structs/VoltageWithRaw.hpp"
#include "Structs/VoltageWithRawAndTime.hpp"
#include "Core/Event.hpp"
#include <atomic>
class LiveDataCache final: public SingletonBase<LiveDataCache>{
public:

	static const char* getTag();
	
	DISALLOW_COPY_MOVE(LiveDataCache);
    // Output Voltage (V)
    VoltageWithRawAndTime getOutputVoltage()const noexcept;

    // Output Current (A)
    FloatAndTime getOutputCurrent()const noexcept;

    // Total Output Energy (J)
    FloatAndTime getTotalOutputEnergy()const noexcept;

    // First Stage Voltage (V)
    VoltageWithRawAndTime getFirstStageVoltage()const noexcept;

private:
    // Let the base construct us
    friend class SingletonBase<LiveDataCache>;
	LiveDataCache(
		Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback,
		Port_OutputVoltageFeedback& portOutputVoltageFeedback
	) noexcept;
	
	Port_FirstStageVoltageFeedback& _portFirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _portOutputVoltageFeedback;
	
    std::atomic<VoltageWithRawAndTime> _outputVoltage{VoltageWithRawAndTime{0.0, 0, 0}};
    std::atomic<FloatAndTime> _outputCurrent{FloatAndTime{0.0, 0}};
    std::atomic<FloatAndTime> _totalOutputEnergy{FloatAndTime{0.0, 0}};
    std::atomic<VoltageWithRawAndTime> _firstStageVoltage{VoltageWithRawAndTime{0.0, 0, 0}};
	EventConnection _eventConnectionGotOutputVoltage;
	EventConnection _eventConnectionGotFirstStageVoltage;
	
    void   setOutputCurrent(float v) noexcept;
    void   setOutputVoltage(VoltageWithRaw v) noexcept;
    void   setFirstStageVoltage(VoltageWithRaw v) noexcept;
    void   setTotalOutputEnergy(float v) noexcept;
};

