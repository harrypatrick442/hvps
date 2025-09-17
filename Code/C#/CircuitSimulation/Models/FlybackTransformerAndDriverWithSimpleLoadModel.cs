using ModifiedNodalAnalysis.SubCircuits;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Nodes;
using ModifiedNodalAnalysis.Components.Bases;
using VoltageMultiplier.CircuitSimulation.Components;

namespace VoltageMultiplier.CircuitSimulation
{
    public class FlybackTransformerAndDriverWithSimpleLoadModel : ModelBase
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
        public IDiode Diode {  get; }
        public BangBangFlybackDriver BangBangFlybackDriver { get; }
        public RCDSnubber Snubber { get; }
        public FlybackTransformerAndDriverWithSimpleLoadModel(
                FullCircuitSimulationArguments args)
        {
            Circuit = new Circuit();
            Ground = new Ground(Circuit);
            NFlybackPrimary1 = new NonGroundNode($"N_fp1", Circuit);
            NFlybackPrimary2 = new NonGroundNode($"N_fp2", Circuit);
            NFlybackSecondary2 = new NonGroundNode($"N_fs2", Circuit);
            NOutput = new NonGroundNode($"N_o", Circuit);
            SupplyVoltageSource  = new FixedVoltageSource(
                "V_in", args.V_in, Ground, NFlybackPrimary1, Circuit);
            double tPeriod = 1d/args.Frequency;
            BangBangFlybackDriver = new BangBangFlybackDriver("Q_1", Ground, NFlybackPrimary2,
                Circuit, args.Frequency, args.D_min, args.D_max, null, null, NOutput, 1000000, 100/*TODO*/, 100,
                t_rise:  tPeriod * 0.01,
                t_fall:  tPeriod * 0.01);
            Snubber = new RCDSnubber(
                "snub", 
                args.R_snubber, 
                args.C_snubber, 
                (name, anode, cathode, circuit)=>new G40H100CTW(name, anode, cathode, circuit),
                nodeCapacitorSize: NFlybackPrimary1,
                nodeDiodeSide: NFlybackPrimary2,
                Circuit);
            FlybackTransformer = new Transformer("flyback",
                NFlybackPrimary1, NFlybackPrimary2, Ground, NFlybackSecondary2,
                nSOverNp: args.NsOverNp,
                cP: 0,
                rP: args.R_p,
                lKp: 0,
                lKs: args.L_ks,
                rC: 0,
                lMp:0,
                lMs: args.L_ms,
                rS: args.R_s,
                cS: 0,
                cM: 0,
                Circuit);
            Diode = new DD1800("diode", NFlybackSecondary2, NOutput, Circuit);
            RLoad = new Resistor("R_load", 20000000, Ground, NOutput, Circuit);
            Capacitor CLoad = new Capacitor("C_load", 1e-7, Ground, NOutput, Circuit);
        }
    }
}