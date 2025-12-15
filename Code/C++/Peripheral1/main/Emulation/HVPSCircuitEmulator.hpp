#pragma once
#include "Enums/SystemState.hpp"
#include "Generated/Peripheral1Config.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Timing/Timer.hpp"
class HVPSCircuitEmulator:public SingletonBase<HVPSCircuitEmulator>{
    friend class SingletonBase<HVPSCircuitEmulator>;
public:
	static inline constexpr const char* TAG = "HVPSCircuitEmulator";
private:
public:
	DISALLOW_COPY_MOVE(HVPSCircuitEmulator);
protected:
	explicit HVPSCircuitEmulator(const Configuration& config)noexcept;
	~HVPSCircuitEmulator();
private:
};