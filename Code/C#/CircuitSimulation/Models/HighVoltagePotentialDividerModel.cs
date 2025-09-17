using ModifiedNodalAnalysis.SubCircuits;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Nodes;
using ModifiedNodalAnalysis.Components.Bases;
using VoltageMultiplier.CircuitSimulation.Components;
using CircuitAnalysis.ComponentSpecs;

namespace VoltageMultiplier.CircuitSimulation.Models
{
    public class HighVoltagePotentialDividerModel : ModelBase
    {
        public override Circuit Circuit { get; }
        public FixedVoltageSource SupplyVoltageSource { get; }
        public Ground Ground { get; }
        public NonGroundNode NHighSide { get; }
        public NonGroundNode NLowCapacitorSmallResistance { get; }
        public NonGroundNode NMiddle { get; }
        public Resistor HighSideResistor { get; }
        public Resistor LowSideResistor { get; }
        public Capacitor HighSideResistorCapacitor { get; }
        public Capacitor LowSideCapacitor { get; }
        public Resistor SmallResistorLowCapacitor { get; }
        public PiecewiseLinearVoltageSource VoltageSource { get; }
        public HighVoltagePotentialDividerModel(
            double voltageReached,
            double duration,
            ResistorSpecs highSideResistor,
            ResistorSpecs lowSideResistor,
            CapacitorSpecs? lowSideCapacitor = null)
        {

            Circuit = new Circuit();
            Ground = new Ground(Circuit);
            NHighSide = new NonGroundNode($"N_h", Circuit);
            NMiddle = new NonGroundNode($"N_m", Circuit);
            NMiddle = new NonGroundNode($"N_m", Circuit);
            NLowCapacitorSmallResistance = new NonGroundNode($"N_h_c", Circuit);
            NLowCapacitorSmallResistance = new NonGroundNode($"N_l_c", Circuit);
            VoltageSource = new PiecewiseLinearVoltageSource("V_s", Ground, NHighSide,
                Circuit,
                0, voltageReached, 0, duration, 0, duration,
                duration, 1);
            HighSideResistor = new Resistor("R_h", highSideResistor.Resistance, NHighSide, NMiddle, Circuit);
            LowSideResistor = new Resistor("R_l", lowSideResistor.Resistance, NMiddle, Ground, Circuit);
            HighSideResistorCapacitor = new Capacitor("C_h", highSideResistor.ParasiticCapacitance,
                NHighSide, NMiddle, Circuit);
            double lowSideCapacitance;
            if (lowSideCapacitor != null)
            {
                lowSideCapacitance = lowSideCapacitor.Capacitance;
            }
            else
            {
                lowSideCapacitance = (highSideResistor.Resistance / lowSideResistor.Resistance) * highSideResistor.ParasiticCapacitance;
                Console.WriteLine("low side capacitance required: " + lowSideCapacitance);
            }
            SmallResistorLowCapacitor = new Resistor("R_c_l_s", 100, NLowCapacitorSmallResistance, Ground, Circuit);
            LowSideCapacitor = new Capacitor("C_l", lowSideCapacitance, NMiddle, NLowCapacitorSmallResistance, Circuit);
        }
    }
}