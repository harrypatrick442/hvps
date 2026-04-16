namespace HVPSCore.Enums
{
    public enum FPGAState
    {
        None = 0,
        Driving= 1,
        SamplingHalfCycle = 2,
        SampledHalfCycle = 3,
        SamplingFullCycle = 4,
        SampledFullCycle = 5,
        NextDataBytes= 6,
    }
}
