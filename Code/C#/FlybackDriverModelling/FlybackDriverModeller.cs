using CircuitAnalysis;
using CircuitAnalysis.ComponentSpecs;

namespace VoltageMultiplier.FlybackTransformerModelling
{
    public class FlybackDriverModeller
    {
        public static void Model(
            MosfetSpecs mosfetSpecs, 
            GateDriverSpecs gateDriverSpecs,
            double Vdsmax,
            double Vdsmin,
            double gateDriverSupplyVoltage,
            double IdMax,
            double switchingFrequency,
            double desiredSwitchingTime,
            double safetyFactor,
            double? gateDriveResistance = null)

        {
            double gateDriveVoltage = gateDriverSupplyVoltage;
            double effectiveGateCapacitance = MOSFETEquations.CalculateEffectiveGateCapacitance(
                C_iss: mosfetSpecs.Ciss,
                C_rss: mosfetSpecs.Crss, Vdsmax/*CHECK*/, gateDriveVoltage);
            if (gateDriveResistance != null)
            {
                Console.WriteLine($"Gate drive resistance provided was: {gateDriveResistance} ohms");
            }
            else
            {
                double proposedGateDriveResistance = MOSFETEquations.CalculateGateResistor(
                    effectiveGateCapacitance, gateDriveVoltage, mosfetSpecs.VgsThresholdMin/*CHECK*/,
                    desiredSwitchingTime);
                gateDriveResistance = MOSFETEquations.ClampGateDriveResistor(
                    switchingFrequency: switchingFrequency,
                    proposedGateDriveResistance,
                    out bool? wasClampedMaxElseMin);
                Console.WriteLine($"Since no gate driver resistance was provided, a value of {gateDriveResistance} was calculated.");
                if (wasClampedMaxElseMin != null) {
                    Console.WriteLine($"This value was clamped to a {(wasClampedMaxElseMin==true?"maximum":"minimum")} value. The unclamped value was: {proposedGateDriveResistance} ohms");
                }
            }
            double maximumInstantaneousPower = MOSFETEquations.CalculateMaximumInstantaneousPower(
                Vdsmax, IdMax);

            double peakGateDriveCurrent = MOSFETEquations.CalculatePeakGateDriveCurrent(
                gateDriveVoltage, (double)gateDriveResistance);
            if (peakGateDriveCurrent * safetyFactor > gateDriverSpecs.Ipk)
            {
                throw new Exception("Peak gate drive current is too large");
            }
            Console.WriteLine($"Peak gate driver current is: {peakGateDriveCurrent}." +
                $" This is {100d * peakGateDriveCurrent / mosfetSpecs.IdPulsedMax}% {nameof(mosfetSpecs.IdPulsedMax)}");
            double gateDriveCurrent = MOSFETEquations.CalculateGateDriveCurrent(
                mosfetSpecs.Ciss, mosfetSpecs.Crss, gateDriveVoltage, switchingFrequency);
            Console.WriteLine($"Average gate drive current is: {gateDriveCurrent} amps");
            double resistorPower = Math.Pow(gateDriveCurrent, 2) * (double)gateDriveResistance;
            Console.WriteLine($"Resistor power is: {resistorPower} watts. Resistor must be rated for at least {resistorPower * safetyFactor} watts");
            double drainCurrent = /*sawtooth triangle wave so half peak. At maximum duty cycle on 50% of time
                                   */IdMax *0.25d;
            double totalPowerLossInMosfet = MOSFETEquations.CalculateTotalPowerLoss(
                drainCurrent: drainCurrent,
                RdsOn: mosfetSpecs.RdsOn,
                Vds: Vdsmax,
                riseTime: mosfetSpecs.RiseTime,
                fallTime: mosfetSpecs.FallTime,
                switchingFrequency: switchingFrequency,
                effectiveGateCapacitance: effectiveGateCapacitance,
                gateDriveVoltage,
            out double proportionLossIsConduction,
            out double proportionLossIsSwitching,
            out double proportionLossIsGateDrive);
            Console.WriteLine($"Total power loss in MOSFET is: {totalPowerLossInMosfet}. {100d*proportionLossIsConduction}% is conduction, "
                +$"{100d*proportionLossIsSwitching}% is switching, and {100d*proportionLossIsGateDrive}% is gate drive.");

        }
    }
}