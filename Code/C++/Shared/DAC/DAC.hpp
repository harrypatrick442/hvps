#pragma once

#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "driver/dac_oneshot.h"

class DAC : public SingletonBase<DAC> {
    friend class SingletonBase<DAC>;
private:
    dac_oneshot_handle_t _ch0;
    dac_oneshot_handle_t _ch1;
public:
	static const char* getTag();
    static inline constexpr float DEFUALT_V_MAX = 3.1;
    DISALLOW_COPY_MOVE(DAC);
	
	void setChannel0Voltage(float voltage, float vMax = DEFUALT_V_MAX);
	void setChannel1Voltage(float voltage, float vMax = DEFUALT_V_MAX);
	void setChannel0Value(uint8_t value);
	void setChannel1Value(uint8_t value);
	static uint8_t voltageToValue(float voltage, float Vmax =DEFUALT_V_MAX);

protected:
    explicit DAC(bool enableChannel0, bool enableChannel1) noexcept;
    ~DAC();

private:
};
