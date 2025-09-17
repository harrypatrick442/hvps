#pragma once
#include <atomic>
#include "../Core/SingletonBase.hpp"
#include "../Core/Macros.hpp"
class Port_FirstStageVoltageFeedback;
class Port_OutputVoltageFeedback;
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
		Port_FirstStageVoltageFeedback& port_firstStageVoltageFeedback,
		Port_OutputVoltageFeedback& port_OutputVoltageFeedback
	) noexcept;
	
	Port_FirstStageVoltageFeedback& _port_firstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _port_OutputVoltageFeedback;
	
    std::atomic<DoubleAndTime> _outputVoltage{0.0};
    std::atomic<DoubleAndTime> _outputCurrent{0.0};
    std::atomic<DoubleAndTime> _totalOutputEnergy{0.0};
    std::atomic<DoubleAndTime> _firstStageVoltage{0.0};
    std::atomic<DoubleAndTime> _peakPrimaryCurrent{0.0};
	EventConnection _eventConnectionGotOutputVoltage;
	EventConnection _eventConnectionGotFirstStageVoltage;
	
    void   setOutputVoltage(double v) noexcept;
    void   setOutputCurrent(double v) noexcept;
	void handleGotOutputVoltage(double voltage) noexcept;
	void handleGotFirstStageVoltage(double voltage) noexcept;
};

#endif // LIVE_DATA_HPP

