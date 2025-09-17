using CircuitAnalysis.ComponentSpecs;
using ModifiedNodalAnalysis;

namespace VoltageMultiplier.FlybackTransformerModelling
{
    public static class VillardCascadingVoltageMultiplierEquations
    {
        public static void Calculate(
            double safetyFactor,
            double frequencyHz,
            double maximumOutputVoltage,
            double maximumSteadyStateCurrent,
            double maximumProportionVoltageRipple,
            DiodeSpecs diodeSpecs,
            CapacitorSpecs capacitorSpecs,
            ResistorSpecs outputResistorSpecs,
            int nOutputResistorsSeries) {
            double minimumOutputResistorRequired = maximumOutputVoltage
                / (diodeSpecs.I_FRM / safetyFactor);
            double totalMinimumOutputResistance = nOutputResistorsSeries
                * outputResistorSpecs.MinimumResistance;
            double totalMaximumOutputResistance = nOutputResistorsSeries
                * outputResistorSpecs.MaximumResistance;
            if (totalMinimumOutputResistance < minimumOutputResistorRequired/safetyFactor)
            {
                bool withoutSafetyFactorToo = totalMinimumOutputResistance < minimumOutputResistorRequired;
                throw new Exception(
                    $"The minimum output resistance required is {minimumOutputResistorRequired}."+
                    $"The selected resistor {outputResistorSpecs.Name} in a series of " +
                    $"{nOutputResistorsSeries} had a minimum value of {totalMinimumOutputResistance}" +
                    $" ohms, {totalMinimumOutputResistance*safetyFactor} including the safety factor" +
                    $"of {safetyFactor}");

            }
            double maximumOutputCurrentPossible = maximumOutputVoltage 
                / totalMinimumOutputResistance;
            double maximumPowerLossPerResistor = Math.Pow(maximumOutputCurrentPossible, 2)
                *totalMinimumOutputResistance/nOutputResistorsSeries;

            Console.WriteLine($"A series of {nOutputResistorsSeries} {outputResistorSpecs.Name}" +
                "resistors in serires gives a total minimum " +
                $"resistance of {totalMinimumOutputResistance} " +
                $"and a maximum resistance of {totalMaximumOutputResistance}." +
                $"This is sufficient to protect the diodes and will allow a maximum " +
                $"current of {maximumOutputCurrentPossible} amps. " +
                $"The maximum power loss per resistor will be: {maximumPowerLossPerResistor} watts");
            double maximumCapacitorVoltageRipple = CalculateMaximumCapacitorVoltageRipple(
                frequencyHz, capacitorSpecs.MinimumCapacitance, maximumSteadyStateCurrent);
            double maximumAllowableVoltageRipple = maximumProportionVoltageRipple * maximumOutputVoltage;
            if (maximumCapacitorVoltageRipple > maximumAllowableVoltageRipple) {
                throw new Exception($"The voltage ripple calculated for a capacitor was " +
                    $"{maximumCapacitorVoltageRipple} volts. This exceeded the maximum voltage " +
                    $"rippler of {maximumAllowableVoltageRipple} volts allowed for a maximum of " +
                    $"{100*maximumProportionVoltageRipple}% ripple");
            }
            Console.WriteLine($"The maximum output voltage ripple will be " +
                $"{maximumCapacitorVoltageRipple} volts which is a ripple of " +
                $"{100*maximumCapacitorVoltageRipple/maximumOutputVoltage}% " +
                $"the maximum output voltage of {maximumOutputVoltage} volts");
        }
        public static double CalculateMaximumCapacitorVoltageRipple(
            double frequencyHz,
            double minimumCapacitance,
            double maximumLoadCurrent
            ) {
            return maximumLoadCurrent / (minimumCapacitance*frequencyHz);
        }
    }
}