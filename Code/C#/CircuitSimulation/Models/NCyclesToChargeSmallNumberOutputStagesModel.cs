using ModifiedNodalAnalysis.SubCircuits;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Nodes;
using ModifiedNodalAnalysis.Components.Bases;
using VoltageMultiplier.CircuitSimulation.Components;

namespace VoltageMultiplier.CircuitSimulation.Models
{
    public class NCyclesToChargeSmallNumberOutputStagesModel : ModelBase
    {
        public override Circuit Circuit { get; }
        public FixedVoltageSource SupplyVoltageSource { get; }
        public Ground Ground { get; }
        public NonGroundNode NFlybackPrimary1 { get; }
        public NonGroundNode NFlybackPrimary2 { get; }
        public NonGroundNode NOutput { get; }
        public NonGroundNode NFlybackSecondary2 { get; }
        public Transformer FlybackTransformer { get; }
        public Resistor RLoad { get; }
        public IDiode FirstStageDiode { get; }
        public IDiode LastStageDiode { get; }
        public Capacitor FirstStageCapacitor { get; }
        public BangBangFlybackDriver BangBangFlybackDriver { get; }
        public VillardCascadeVoltageMultiplier VoltageMultiplier { get; }
        public RCDSnubber Snubber { get; }
        public RCDSnubber Snubber2 { get; }
        public NCyclesToChargeSmallNumberOutputStagesModel(
            int nVillardCascadingHalfStages,
            double? outputResistance,
                FullCircuitSimulationArguments args)
        {

            Circuit = new Circuit();
            Ground = new Ground(Circuit);
            NFlybackPrimary1 = new NonGroundNode($"N_fp1", Circuit);
            NFlybackPrimary2 = new NonGroundNode($"N_fp2", Circuit);
            NFlybackSecondary2 = new NonGroundNode($"N_fs2", Circuit);
            NOutput = new NonGroundNode($"N_o", Circuit);
            SupplyVoltageSource = new FixedVoltageSource(
                "V_in", args.V_in, Ground, NFlybackPrimary1, Circuit);
            double tPeriod = 1d / args.Frequency;
            BangBangFlybackDriver = new BangBangFlybackDriver("Q_1", Ground, NFlybackPrimary2,
                Circuit, args.Frequency, args.D_min, args.D_max, null, null, NOutput, 1000000, 100/*TODO*/, args.MaximumFirstStageVoltageForBangBang,
                t_rise: tPeriod * 0.01,
                t_fall: tPeriod * 0.01
                                   );
            Snubber = new RCDSnubber(
                "snub",
                args.R_snubber,
                args.C_snubber,
                (name, anode, cathode, circuit) => new G40H100CTW(name, anode, cathode, circuit),
                nodeCapacitorSize: NFlybackPrimary1,
                nodeDiodeSide: NFlybackPrimary2,
                Circuit);
            FlybackTransformer = new Transformer("flyback",
                NFlybackPrimary1, NFlybackPrimary2, Ground, NFlybackSecondary2,
                nSOverNp: args.NsOverNp,
                cP: 0,
                rP: args.R_p,
                lKp: args.L_ks / Math.Pow(args.NsOverNp, 2),
                lKs: 0,
                rC: 0,
                lMp: args.L_ms/Math.Pow(args.NsOverNp, 2),
                lMs: 0,
                rS: args.R_s,
                cS: 0,
                cM: 0,
                Circuit);
            VoltageMultiplier = new VillardCascadeVoltageMultiplier(
                name: "vm",
                nHalfStages: nVillardCascadingHalfStages,
                capacitorCapacitance: args.Capacitance,
                dischargeResistorResistance: args.DischargeResistorResistance,
                createDiode: (name, anode, cathode, components)
                => new DD1800(name, anode, cathode, components),
                nInA: Ground,
                nInBFirstDiodeAnode: NFlybackSecondary2,
                nOut: NOutput,
                Circuit);
            if (outputResistance != null)
            {
                RLoad = new Resistor("R_load", (double)outputResistance, Ground, NOutput, Circuit);

            }
            FirstStageDiode = VoltageMultiplier.Diodes.First();
            LastStageDiode = VoltageMultiplier.Diodes.Last();
            FirstStageCapacitor = VoltageMultiplier.Capacitors.First();
        }
    }
}