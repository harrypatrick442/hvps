#pragma once
class IOutputs {
public:
    virtual void toSafe() noexcept = 0;  // pure virtual, must be overridden
    virtual ~IOutputs() = default;       // safe polymorphic cleanup
};