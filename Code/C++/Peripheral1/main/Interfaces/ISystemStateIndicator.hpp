#pragma once
#include "Enums/SystemState.hpp"
class ISystemStateIndicator {
public:
    virtual ~ISystemStateIndicator() = default;
    virtual void indicateState(SystemState systemState) noexcept = 0;
};