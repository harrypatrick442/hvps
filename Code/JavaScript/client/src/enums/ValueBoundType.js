const ValueBoundType = {
    Exact: 1,             // Known, precise value
    MinimumKnown: 2, // This is a known lower bound
    MaximumKnown: 3, // This is a known upper bound
    Approximate: 4, // Estimate
    LowerBound: 5,   // Explicit range lower bound
    UpperBound: 6,   // Explicit range upper bound
    Unknown: 0          // No reliable qualifier
};

ValueBoundType.getDescription = (value, boundType) => {
    switch (boundType) {
        case ValueBoundType.Exact:
            return `Exactly ${value}`;
        case ValueBoundType.MinimumKnown:
            return `At least ${value}`;
        case ValueBoundType.MaximumKnown:
            return `At most ${value}`;
        case ValueBoundType.Approximate:
            return `Approximately ${value}`;
        case ValueBoundType.LowerBound:
            return `Lower bound: ${value}`;
        case ValueBoundType.UpperBound:
            return `Upper bound: ${value}`;
        case ValueBoundType.Unknown:
        default:
            return `Value unknown`;
    }
};
export default ValueBoundType;
