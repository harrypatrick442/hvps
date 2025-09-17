namespace VoltageMultiplier.CircuitSimulation
{
    public class PIControllerConstants
    {
        public double PSecondaryCurrent { get; }
        public double ISecondaryCurrent { get; }
        public double PFirstStageVoltage { get; }
        public double IFirstStageVoltage { get; }
        public double POutputVoltage { get; }
        public double IOutputVoltage { get; }
        public PIControllerConstants(
            double pSecondaryCurrent,
            double iSecondaryCurrent,
            double pFirstStageVoltage,
            double iFirstStageVoltage,
            double pOutputVoltage,
            double iOutputVoltage)
        {
            PSecondaryCurrent = pSecondaryCurrent;
            ISecondaryCurrent = iSecondaryCurrent;
            PFirstStageVoltage = pFirstStageVoltage;
            IFirstStageVoltage = iFirstStageVoltage;
            POutputVoltage = pOutputVoltage;
            IOutputVoltage = iOutputVoltage;
        }
    }
}