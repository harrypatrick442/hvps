using Core;
using Core.Maths.Tolerances;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Components.Bases;
using ModifiedNodalAnalysis.Interfaces;
using ModifiedNodalAnalysis.Results;
using ModifiedNodalAnalysis.Solver;
using VoltageMultiplier.CircuitSimulation.Models;
using VoltageMultiplier.CircuitSimulation.Simulations;
using _Args = ModifiedNodalAnalysis.FullCircuitSimulationArguments;

namespace VoltageMultiplier.CircuitSimulation
{
    public class NCyclesToChargeSmallNumberOutputStagesSimulation
        : SimulationBase<NCyclesToChargeSmallNumberOutputStagesModel, _Args, NCyclesToChargeSmallNumberOutputStagesResults>
    {
        private int _NVillardCascadingHalfStages,
            _NCycles;
        private double? _OutputResistance;
        public NCyclesToChargeSmallNumberOutputStagesSimulation(
            int nVillardCascadingHalfStages, 
            double? outputResistance,
            int nCycles)
            : base(useGPU:false)
        {
            _NCycles = nCycles;
            _NVillardCascadingHalfStages = nVillardCascadingHalfStages;
            _OutputResistance = outputResistance;
        }
        protected override NCyclesToChargeSmallNumberOutputStagesResults? ProcessResults
            (string resultsDirectory,
            _Args args, 
            NCyclesToChargeSmallNumberOutputStagesModel model, 
            ResultsHandler resultsHandler)
        {
            Capacitor firstCapacitor = model.VoltageMultiplier.Capacitors.First();
            firstCapacitor.Voltage.GetAverageMaxMin(resultsHandler, 
                out double averageOutputVoltage, out double maxFirstStageCapacitorVoltageReached, out int maxFirstStageCapacitorVoltageReachedIndex, 
                out double minFirstStageCapacitorVoltageReached, out int minFirstStateCapacitorVoltageReachedIndex);
            double maxFirstStateCapacitorVoltageReachedTime = resultsHandler.GetTimeFromIndex(maxFirstStageCapacitorVoltageReachedIndex);
            double minFirstStateCapacitorVoltageReachedTime = resultsHandler.GetTimeFromIndex(minFirstStateCapacitorVoltageReachedIndex);
            IDiode firstDiode = model.VoltageMultiplier.Diodes.First();
            firstDiode.Current.GetAverageMaxMin(resultsHandler, out double diodeAverageCurrent,
                out double firstStageDiodeMaxCurrent, out int firstStageDiodeMaxCurrentIndex, 
                out double firstStageDiodeMinCurrent, out int firstStageDiodeMinCurrentIndex);
            double firstStageDiodeMaxCurrentTime = resultsHandler.GetTimeFromIndex(firstStageDiodeMaxCurrentIndex);
            double firstStageDiodeMinCurrentTime = resultsHandler.GetTimeFromIndex(firstStageDiodeMinCurrentIndex);
            firstDiode.Voltage.GetAverageMaxMin(resultsHandler, out double firstStageDiodeAverageVoltage,
                out double firstStageDiodeMaxVoltage, out int firstStageDiodeMaxVoltageIndex,
                out double firstStageDiodeMinVoltage, out int firstStageDiodeMinVoltageIndex);
            double firstStageDiodeMaxVoltageTime = resultsHandler.GetTimeFromIndex(firstStageDiodeMaxVoltageIndex);
            double firstStageDiodeMinVoltageTime = resultsHandler.GetTimeFromIndex(firstStageDiodeMinVoltageIndex);
            return new NCyclesToChargeSmallNumberOutputStagesResults(
                maxFirstStageCapacitorVoltageReached,
                maxFirstStateCapacitorVoltageReachedTime,
                minFirstStageCapacitorVoltageReached,
                minFirstStateCapacitorVoltageReachedTime,
                firstStageDiodeMaxCurrent,
                firstStageDiodeMaxCurrentTime,
                firstStageDiodeMinCurrent,
                firstStageDiodeMinCurrentTime,
                firstStageDiodeMaxVoltage,
                firstStageDiodeMaxVoltageTime,
                firstStageDiodeMinVoltage,
                firstStageDiodeMinVoltageTime
                );
        }
        protected override NCyclesToChargeSmallNumberOutputStagesModel CreateModel(_Args args)
        {
            return new NCyclesToChargeSmallNumberOutputStagesModel(
                _NVillardCascadingHalfStages, _OutputResistance, args);
        }
            
        protected override MNANewtonRaphsonStoppingParameters CreateNewtonRaphsonStoppingParameters()
        {
            return new MNANewtonRaphsonStoppingParameters(
                    maximumRelativeTolerance: 10000,
                    desiredMaximumRelativeTolerance: 1,
                    targetRelativeTolerance: 1,
                    relativeToleranceWeight: 1,
                    absoluteToleranceVoltage: new AbsoluteTolerance(
                            targetTolerance: 0.1,//0.01,
                            desiredMaxTolerance: 1,
                            maxTolerance: 10000,
                            weight: 1
                        ),
                    absoluteToleranceCurrent: new AbsoluteTolerance(
                            targetTolerance: 0.01,//0.001,
                            desiredMaxTolerance: 1,
                            maxTolerance: 10000,
                            weight: 1
                        ),
                    maximumResidualTolerance: 10000,
                    desiredMaximumResidualTolerance: 10,
                    targetResidualTolerance: 1,
                    residualToleranceWeight: 1,
                    maxConvergenceSteps: 60,//500
                    failIfExceedsMaxConvergenceSteps: true,
                    residualCurrentOverOldestStagnationThreshold:0.9999999999,
                    stepCurrentOverOldestStagnationThreshold: 0.9999999999
                );
        }

        protected override IStoppingCondition[] CreateStoppingConditions(
            _Args args, NCyclesToChargeSmallNumberOutputStagesModel model)
        {
            return new IStoppingCondition[] {
                new StopAtTime((double)_NCycles/args.Frequency)
            };
        }

        protected override ITimestepParams CreateTimeStepController()
        {
            return new TimestepParams(
                minTimestep: 1e-19,
                maxTimestep: 1e-9,
                growthShrinkFactor: 0.5,
                maximumAllowedError: 100,
                targetError: 0.01);
        }

        protected override Tuple<string, IProperty>[] GetNameAndPropertyToPlots(
            NCyclesToChargeSmallNumberOutputStagesModel model)
        {
            var tupleList = new TupleList<string, IProperty> {
                { "snubber_capacitor_voltage", model.Snubber.Capacitor.Voltage},
                { "snubber_capacitor_current", model.Snubber.Capacitor.Current},
                { "snubber_diode_voltage", model.Snubber.Diode.Voltage},
                { "snubber_diode_current", model.Snubber.Diode.Current},
                { "snubber_resistor_voltage", model.Snubber.Resistor.Voltage},
                { "snubber_resistor_current", model.Snubber.Resistor.Current},
                { "duty_cycle", model.BangBangFlybackDriver.DutyCycle },
                { "driver_resistance", model.BangBangFlybackDriver.Resistance },
                { "voltage_source_voltage", model.SupplyVoltageSource.Voltage },
                { "voltage_source_current", model.SupplyVoltageSource.Current },
                { "voltage_source_power", model.SupplyVoltageSource.Power},
                { "flyback_primary_voltages", model.NFlybackPrimary1 },
                { "flyback_secondary_voltages",  model.NFlybackSecondary2 },
                { "flyback_primary_current", model.FlybackTransformer.Rp!.Current },
                { "flyback_secondary_current", model.FlybackTransformer.Rs!.Current },
                { "flyback_secondary_voltage",  model.NFlybackSecondary2 },
                { "output_node_voltage", model.NOutput },
                { "first_stage_capacitor_current", model.FirstStageCapacitor.Current},
                { "first_stage_capacitor_voltage",  model.FirstStageCapacitor.Voltage},
                { "first_stage_capacitor_power",  model.FirstStageCapacitor.Power},
                { "first_stage_diode_current", model.FirstStageDiode.Current},
                { "first_stage_diode_voltage",  model.FirstStageDiode.Voltage},
                { "first_stage_diode_power",  model.FirstStageDiode.Power},
                { "last_stage_diode_current", model.LastStageDiode.Current},
                { "last_stage_diode_voltage",  model.LastStageDiode.Voltage},
                { "last_stage_diode_power",  model.LastStageDiode.Power},
                { "ideal_transformer_secondary_voltage",  model.FlybackTransformer.Ts.SecondaryVoltage},
                { "ideal_transformer_primary_voltage",  model.FlybackTransformer.Ts.PrimaryVoltage},
                { "flyback_primary_mutual_inducatnace_voltage",  model.FlybackTransformer.Lmp.Voltage},
                { "flyback_primary_mutual_inducatnace_current",  model.FlybackTransformer.Lmp.Current},
                { "flyback_primary_leakage_inductance_voltage",  model.FlybackTransformer.Lkp.Voltage},
                { "flyback_primary_leakage_inductance_current",  model.FlybackTransformer.Lkp.Current},
                { "flyback_primary_resistance_voltage",  model.FlybackTransformer.Rp.Voltage},
                { "flyback_primary_resistance_current",  model.FlybackTransformer.Rp.Current},
                { "flyback_secondary_resistance_voltage",  model.FlybackTransformer.Rs.Voltage},
                { "flyback_secondary_resistance_current",  model.FlybackTransformer.Rs.Current},
            };
            if (model.RLoad != null) {
                tupleList.Add("r_load_voltage", model.RLoad.Voltage);
                tupleList.Add("r_load_current", model.RLoad.Current);
                tupleList.Add("r_load_power", model.RLoad.Power);
            }
            return tupleList.ToArray();
        }
    }
}