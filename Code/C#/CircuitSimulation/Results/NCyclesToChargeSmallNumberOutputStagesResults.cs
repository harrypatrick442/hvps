namespace VoltageMultiplier.CircuitSimulation
{
    public class NCyclesToChargeSmallNumberOutputStagesResults
    {
        public double MaxFirstStageCapacitorVoltageReached { get; }
        public double MaxFirstStateCapacitorVoltageReachedTime { get; }
        public double MinFirstStageCapacitorVoltageReached { get; }
        public double MinFirstStateCapacitorVoltageReachedTime { get; }
        public double FirstStageDiodeMaxCurrent { get; }
        public double FirstStageDiodeMaxCurrentTime { get; }
        public double FirstStageDiodeMinCurrent { get; }
        public double FirstStageDiodeMinCurrentTime { get; }
        public double FirstStageDiodeMaxVoltage { get; }
        public double FirstStageDiodeMaxVoltageTime { get; }
        public double FirstStageDiodeMinVoltage { get; }
        public double FirstStageDiodeMinVoltageTime { get; }
        public NCyclesToChargeSmallNumberOutputStagesResults(
            double maxFirstStageCapacitorVoltageReached,
            double maxFirstStateCapacitorVoltageReachedTime,
            double minFirstStageCapacitorVoltageReached,
            double minFirstStateCapacitorVoltageReachedTime,
            double firstStageDiodeMaxCurrent,
            double firstStageDiodeMaxCurrentTime,
            double firstStageDiodeMinCurrent,
            double firstStageDiodeMinCurrentTime,
            double firstStageDiodeMaxVoltage,
            double firstStageDiodeMaxVoltageTime,
            double firstStageDiodeMinVoltage,
            double firstStageDiodeMinVoltageTime)
        {


            MaxFirstStageCapacitorVoltageReached = maxFirstStageCapacitorVoltageReached;
            MaxFirstStateCapacitorVoltageReachedTime = maxFirstStateCapacitorVoltageReachedTime;
            MinFirstStageCapacitorVoltageReached = minFirstStageCapacitorVoltageReached;
            MinFirstStateCapacitorVoltageReachedTime = minFirstStateCapacitorVoltageReachedTime;
            FirstStageDiodeMaxCurrent = firstStageDiodeMaxCurrent;
            FirstStageDiodeMaxCurrentTime = firstStageDiodeMaxCurrentTime;
            FirstStageDiodeMinCurrent = firstStageDiodeMinCurrent;
            FirstStageDiodeMinCurrentTime= firstStageDiodeMinCurrentTime;
            FirstStageDiodeMaxVoltage = firstStageDiodeMaxVoltage;
            FirstStageDiodeMaxVoltageTime = firstStageDiodeMaxVoltageTime;
            FirstStageDiodeMinVoltage = firstStageDiodeMinVoltage;
            FirstStageDiodeMinVoltageTime = firstStageDiodeMinVoltageTime;
        }
    }
}