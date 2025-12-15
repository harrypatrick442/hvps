#pragma once

#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "driver/dac.h"

class DAC : public SingletonBase<DAC> {
    friend class SingletonBase<DAC>;
public:
    static inline constexpr const char* TAG = "DAC";
    static inline constexpr float DEFUALT_V_MAX = 3.1;
    DISALLOW_COPY_MOVE(DAC);
	
	void setChannel1Voltage(float voltage, float vMax = DEFUALT_V_MAX);
	void setChannel2Voltage(float voltage, float vMax = DEFUALT_V_MAX);
	void setChannelVoltage(dac_channel_t channel, float voltage, float vMax = DEFUALT_V_MAX);
	void setChannel1Value(uint8_t value);
	void setChannel2Value(uint8_t value);
	void setChannelValue(dac_channel_t channel, uint8_t value);
	static uint8_t voltageToValue(float voltage, float Vmax =DEFUALT_V_MAX);

protected:
    explicit DAC(bool enableChannel1, bool enableChannel2) noexcept;
    ~DAC();

private:
};
