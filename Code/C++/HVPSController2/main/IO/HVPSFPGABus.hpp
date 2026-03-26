#pragma once
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "FPGA/FPGABus.hpp"
class HVPSFPGABus final:
	public FPGABus,
	public SingletonBase<HVPSFPGABus>
{
    friend class SingletonBase<HVPSFPGABus>;
public:
	static const char* getTag();
    // Force derived classes to identify themselves
	
	DISALLOW_COPY_MOVE(HVPSFPGABus);
protected:
    explicit HVPSFPGABus()noexcept;
};