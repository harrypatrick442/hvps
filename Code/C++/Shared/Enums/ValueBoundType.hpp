#pragma once

enum class ValueBoundType : uint8_t 
{
    Unknown = 0,// No reliable qualifier
    Exact = 1,// Known, precise value
    MinimumKnown = 2,// This is a known lower bound
    MaximumKnown = 3,// This is a known upper bound
    Approximate = 4,// Estimate
    LowerBound = 5,// Explicit range lower bound
    UpperBound = 6// Explicit range upper bound
};
