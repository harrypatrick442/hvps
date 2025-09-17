using CircuitAnalysis.ComponentSpecs;
using Core;
using Core.Maths.Tolerances;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Components.Bases;
using ModifiedNodalAnalysis.Interfaces;
using ModifiedNodalAnalysis.Results;
using ModifiedNodalAnalysis.Solver;
using VoltageMultiplier.CircuitSimulation.Models;
using VoltageMultiplier.CircuitSimulation.Simulations;
using _Args = VoltageMultiplier.CircuitSimulation.Args.HighVoltagePotentialDividerSimulationArgs;

namespace VoltageMultiplier.CircuitSimulation
{
    public class HighVoltagePotentialDividerSimualtion
        : SimulationBase<HighVoltagePotentialDividerModel, _Args, HighVoltagePotentialDividerResults>
    {
        private double _VoltageReached, _Duration;
        private ResistorSpecs _HighSideResistor, _LowSideResistor;
        public HighVoltagePotentialDividerSimualtion(
            double voltageReached, double duration,
            ResistorSpecs highSideResistor, ResistorSpecs lowSideResistor)
            : base(useGPU:false)
        {
            _VoltageReached = voltageReached;
            _Duration = duration;
            _HighSideResistor = highSideResistor;
            _LowSideResistor = lowSideResistor;
        }
        protected override HighVoltagePotentialDividerResults? ProcessResults
            (string resultsDirectory,
            _Args args,
            HighVoltagePotentialDividerModel model, 
            ResultsHandler resultsHandler)
        {
            return new HighVoltagePotentialDividerResults(
                );
        }
        protected override HighVoltagePotentialDividerModel CreateModel(_Args args)
        {
            return new HighVoltagePotentialDividerModel(_VoltageReached, _Duration, _HighSideResistor, _LowSideResistor);
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
            _Args args, HighVoltagePotentialDividerModel model)
        {
            return new IStoppingCondition[] {
                new StopAtTime(_Duration)
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
            HighVoltagePotentialDividerModel model)
        {
            var tupleList = new TupleList<string, IProperty> {
                { "voltage_source_voltage", model.VoltageSource.Voltage},
                { "voltage_source_current", model.VoltageSource.Current},
                { "high_side_resistor_voltage", model.HighSideResistor.Voltage},
                { "high_side_resistor_current", model.HighSideResistor.Current},
                { "high_side_resistor_power", model.HighSideResistor.Power},
                { "high_side_capacitor_voltage", model.HighSideResistorCapacitor.Voltage},
                { "high_side_capacitor_current", model.HighSideResistorCapacitor.Current},
                { "low_side_resistor_voltage", model.LowSideResistor.Voltage},
                { "low_side_resistor_current", model.LowSideResistor.Current}
            };
            return tupleList.ToArray();
        }
    }
}