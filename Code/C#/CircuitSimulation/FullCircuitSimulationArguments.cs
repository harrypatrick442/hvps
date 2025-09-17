using CircuitAnalysis;
using CircuitAnalysis.ComponentSpecs;
using Core.ThisSystem;
using VoltageMultiplier.CircuitSimulation;
using VoltageMultiplier.FlybackTransformerModelling;
using Paths = VoltageMultiplier.FlybackTransformerModelling.Paths;
namespace ModifiedNodalAnalysis
{
    public struct FullCircuitSimulationArguments
    {
        private double _SafetyFactor;
        private Paths _Paths;
        private BasicCalculationsFlybackSpecs _BasicCalculationsFlybackSpecs;
        private FEMResultFlybackSpecs _FEMResultFlybackSpecs;
        private ResistorSpecs _VillardCapacitorDischargeResistorSpecs;
        private DiodeSpecs _VillardDiodeSpecs;
        private CapacitorSpecs _VillardCapacitorSpecs;
        private ResistorSpecs _SecondaryCurrentSenseResistorSpecs;
        private ResistorSpecs _VillardOutputCurrentLimitingResistor;
        private RCDSnubberSpecs _SnubberSpecs;
        public double Frequency { get; }
        public double V_in { get; }
        public double NsOverNp => _BasicCalculationsFlybackSpecs.NsOverNp;
        public double L_ms => _FEMResultFlybackSpecs.L_ms;
        public double L_ks => _FEMResultFlybackSpecs.L_ks;
        public double R_p => _BasicCalculationsFlybackSpecs.R_p;
        public double R_s => _BasicCalculationsFlybackSpecs.R_s;
        public double DischargeResistorResistance => _VillardCapacitorDischargeResistorSpecs.Resistance;
        public double Capacitance => _VillardCapacitorSpecs.Capacitance;
        public string ResultsDirectory => _Paths.CircuitSimulationResultsDirectory;
        public double R_s_sense => _SecondaryCurrentSenseResistorSpecs.Resistance;
        public double R_v_output => _SecondaryCurrentSenseResistorSpecs.Resistance;
        public double R_snubber => _SnubberSpecs.R;
        public double C_snubber => _SnubberSpecs.C;
        public double D_min { get; }
        public double D_max { get; }
        public double MaximumFlybackSecondaryCurrentForBangBang => _VillardDiodeSpecs.I_FRM / _SafetyFactor;
        public double MaximumFirstStageVoltageForBangBang => _VillardCapacitorSpecs.VoltageRating / _SafetyFactor;
        public int NMultiplierStages { get; }
        public PIControllerConstants PiControllerConstants { get; }
        public double MaximumSafeOperatingVillardCapacitorVoltage =>
            _VillardCapacitorSpecs.VoltageRating / _SafetyFactor;
        public double MaximumSafeOperatingVillardDiodeVoltage =>
            _VillardDiodeSpecs.V_rrm / _SafetyFactor;
        public double MaximumSafeOperatingVillardDiodeCurrent =>
            _VillardDiodeSpecs.I_FRM / _SafetyFactor;
        public double MaximumFlybackSecondaryCurrent => _BasicCalculationsFlybackSpecs.I_s_peak;
        public bool UseCachedResults { get;}
        public FullCircuitSimulationArguments(
            double safetyFactor,
            double frequency,
            double V_in,
            double D_min,
            double D_max,
            int nMultiplierStages,
        BasicCalculationsFlybackSpecs basicCalculationsFlybackSpecs,
            PIControllerConstants piControllerConstants,
            FEMResultFlybackSpecs femResultFlybackSpecs,
            ResistorSpecs villardCapacitorDischargeResistorSpecs,
            CapacitorSpecs villardCapacitorSpecs,
            DiodeSpecs villardDiodeSpecs,
            ResistorSpecs villardOutputCurrentLimitingResistor,
            ResistorSpecs secondaryCurrentSenseResistorSpecs,
            RCDSnubberSpecs snubberSpecs,
            Paths paths,
            bool useCachedResults) {
            _SafetyFactor = safetyFactor;
            _BasicCalculationsFlybackSpecs = basicCalculationsFlybackSpecs;
            PiControllerConstants = piControllerConstants;
            _FEMResultFlybackSpecs= femResultFlybackSpecs;
            _VillardCapacitorDischargeResistorSpecs = villardCapacitorDischargeResistorSpecs;
            _VillardCapacitorSpecs= villardCapacitorSpecs;
            _VillardDiodeSpecs = villardDiodeSpecs;
            _SecondaryCurrentSenseResistorSpecs = secondaryCurrentSenseResistorSpecs;
            _VillardOutputCurrentLimitingResistor = villardOutputCurrentLimitingResistor;
            _SnubberSpecs = snubberSpecs;
            _Paths = paths;
            NMultiplierStages = nMultiplierStages;
            this.Frequency = frequency;
            this.V_in= V_in;
            this.D_min = D_min;
            this.D_max = D_max;
            UseCachedResults = useCachedResults;
        }
    }
}
