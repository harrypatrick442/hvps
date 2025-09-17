using ModifiedNodalAnalysis.Solver;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Interfaces;
using _Args = ModifiedNodalAnalysis.FullCircuitSimulationArguments;
using Core.Maths.Tolerances;
using VoltageMultiplier.CircuitSimulation.Simulations;
using Core;

namespace VoltageMultiplier.CircuitSimulation
{
    public class FlybackTransformerAndDriverWithSimpleLoadSimulation 
        : SimulationBase<FlybackTransformerAndDriverWithSimpleLoadModel, FullCircuitSimulationArguments>
    {
        public FlybackTransformerAndDriverWithSimpleLoadSimulation()
            : base()
        {
        }

        protected override FlybackTransformerAndDriverWithSimpleLoadModel CreateModel(_Args args)
        {
            return new FlybackTransformerAndDriverWithSimpleLoadModel(args);
        }

        protected override MNANewtonRaphsonStoppingParameters CreateNewtonRaphsonStoppingParameters()
        {
            return new MNANewtonRaphsonStoppingParameters(
                    maximumRelativeTolerance: 100000,
                    desiredMaximumRelativeTolerance: 1,
                    targetRelativeTolerance: 1,
                    relativeToleranceWeight: 1,
                    absoluteToleranceVoltage: new AbsoluteTolerance(
                            targetTolerance: 1,
                            desiredMaxTolerance: 1,
                            maxTolerance: 100000,
                            weight: 1
                        ),
                    absoluteToleranceCurrent: new AbsoluteTolerance(
                            targetTolerance: 1e-1,
                            desiredMaxTolerance: 1,
                            maxTolerance: 100000,
                            weight: 1
                        ),
                    maximumResidualTolerance: 10000000000,
                    desiredMaximumResidualTolerance: 10,
                    targetResidualTolerance: 1,
                    residualToleranceWeight: 1,
                    maxConvergenceSteps: 1000,
                    failIfExceedsMaxConvergenceSteps: true
                );
        }

        protected override IStoppingCondition[] CreateStoppingConditions(_Args args, FlybackTransformerAndDriverWithSimpleLoadModel model)
        {
            return new IStoppingCondition[] {
                new StopAtTime(3d/args.Frequency)
            };
        }

        protected override ITimestepParams CreateTimeStepController()
        {
            return new TimestepParams(
                minTimestep: 1e-18,
                maxTimestep: 1e-7,
                growthShrinkFactor: 0.5,
                maximumAllowedError: 1,
                targetError: 0.01,
                allowBacktrackOnLessThanMinTimestep: false);
        }

        protected override Tuple<string, IProperty>[] GetNameAndPropertyToPlots(FlybackTransformerAndDriverWithSimpleLoadModel model)
        {
            return new TupleList<string, IProperty> {
                { "duty_cycle", model.BangBangFlybackDriver.DutyCycle },
                { "driver_resistance", model.BangBangFlybackDriver.Resistance },
                { "voltage_source_voltage", model.SupplyVoltageSource.Voltage },
                { "voltage_source_current", model.SupplyVoltageSource.Current },
                { "voltage_source_power", model.SupplyVoltageSource.Power},
                { "flyback_primary_voltages", model.NFlybackPrimary1 },
                { "flyback_secondary_voltages",  model.NFlybackSecondary2 },
                { "output_node_voltage", model.NOutput },
                { "r_load_voltage",  model.RLoad.Voltage},
                { "r_load_current",  model.RLoad.Current},
                { "r_load_power",  model.RLoad.Power},
                { "diode_current", model.Diode.Current},
                { "diode_voltage",  model.Diode.Voltage},
                { "diode_power",  model.Diode.Power},
            }.ToArray();
        }
    }
}