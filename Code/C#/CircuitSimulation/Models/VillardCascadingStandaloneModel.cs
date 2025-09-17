using ModifiedNodalAnalysis.SubCircuits;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Nodes;

namespace VoltageMultiplier.CircuitSimulation.Models
{
    public class VillardCascadingStandaloneModel : ModelBase
    {
        public override Circuit Circuit { get; }
        public PiecewiseLinearVoltageSource SupplyVoltageSource { get; }
        public Ground Ground { get; }
        public NonGroundNode NIn { get; }
        public NonGroundNode NOutput { get; }
        public Resistor RLoad { get; }
        public VillardCascadeVoltageMultiplier VoltageMultiplier { get; }
        public VillardCascadingStandaloneModel(
                FullCircuitSimulationArguments args)
        {

            Circuit = new Circuit();
            Ground = new Ground(Circuit);
            NIn = new NonGroundNode($"N_fp1", Circuit);
            NOutput = new NonGroundNode($"N_o", Circuit);
            double tPeriod = 1d / args.Frequency;
            SupplyVoltageSource = new PiecewiseLinearVoltageSource(
                "V_in", Ground, NIn, Circuit, 0,
                vOn: 14000,
                tDelay: 0,
                tRise: 0,
                tFall: 0,
                tOn: tPeriod / 2d,
                tPeriod: tPeriod);
            VoltageMultiplier = new VillardCascadeVoltageMultiplier(
                name: "vm",
                nHalfStages: args.NMultiplierStages,
                capacitorCapacitance: args.Capacitance,
                dischargeResistorResistance: args.DischargeResistorResistance,
                createDiode: (name, anode, cathode, components)
                => new DD1800(name, anode, cathode, components),
                nInA: Ground,
                nInBFirstDiodeAnode: NIn,
                nOut: NOutput,
                Circuit);
            RLoad = new Resistor("R_load", 50000000000, Ground, NOutput, Circuit);
        }
    }
}