using ModifiedNodalAnalysis.Solver;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Interfaces;
using _Args = ModifiedNodalAnalysis.FullCircuitSimulationArguments;
using Core.Maths.Tolerances;
using VoltageMultiplier.CircuitSimulation.Models;
using Core;

namespace VoltageMultiplier.CircuitSimulation.Simulations
{
    public class FullSimulation : SimulationBase<FullModel, FullCircuitSimulationArguments>
    {
        public FullSimulation():base() {

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

        protected override Tuple<string, IProperty>[] GetNameAndPropertyToPlots(FullModel model)
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
        protected override FullModel CreateModel(_Args args)
        {
            return new FullModel(args);
        }

        protected override IStoppingCondition[] CreateStoppingConditions(_Args args, FullModel model)
        {
            return new IStoppingCondition[] {
               new StopAtTime(300d/args.Frequency)
            };
        }
    }
}







/*
        public static void Debug(
            Args args,
            double maximumPowerOutputDesired,
            double minimumOutputVoltageDesired,
            double maximumOutputVoltageDesired
            )
        {
            double desiredOutputVoltage = maximumOutputVoltageDesired;
            double desiredPowerOutput = maximumPowerOutputDesired;
            Solve("debug flyback",
                args,
                desiredPowerOutput,
                desiredOutputVoltage,
                (model, resultsHandler, resultNameNormalized)
                    => PlotResults(args, model, resultsHandler, resultNameNormalized),
                (model) =>
                {
                    return new IStoppingCondition[] {
                        new StopAtTime(300d/args.Frequency)
                    };
                });
        }
        private static void Solve(
        string solutionName,
        Args args,
        double desiredPowerOutput,
        double desiredOutputVoltage,
        DelegateUseResults useResults,
        Func<DebugFlybackModel, IStoppingCondition[]> createStoppingConditions
        )
        {

            double currentOutput = desiredPowerOutput / desiredOutputVoltage;
            double loadResistance = desiredOutputVoltage / currentOutput;
            DebugFlybackModel model = new DebugFlybackModel(
                args);
            IStoppingCondition[] stoppingConditions = createStoppingConditions(model);
            if (!stoppingConditions.Any())
                throw new ArgumentException($"{nameof(stoppingConditions)} had a length of zero. You must provide stopping conditions");
            IStoppingCondition stoppingCondition = stoppingConditions.Length > 1
                ? new CompositeStoppingCondition(stoppingConditions, stoppingConditions.Take(1).ToArray()) : stoppingConditions[0];
            string resultFileNameWithoutExtensionNormalized = FileHelper.NormalizeFileNameLowerCaseUnderscore(solutionName);
            using (CudaContextAssignedThreadPool cudaContextAssignedThreadPool =
                new CudaContextAssignedThreadPool(1))
            {
                UsingResultsHandler(args,
                    resultFileNameWithoutExtensionNormalized,
                    model,
                    (resultsHandler) =>
                {
                    HandleProgress(solutionName, (progressHandler) =>
                    {
                        try
                        {
                            MNASolver.SolveOverTimeDomain(
                                model.Circuit,
                                stoppingCondition,
                                resultsHandler,
                                //0.000156208499998507,//
                                startTime: args.UseCachedResults || true ? resultsHandler.EndTimeInclusive : 0,
                                progressHandler: progressHandler,
                                timestepController: new TimeStepController(
                                    minTimestep: 1e-18,
                                    maxTimestep: 1e-7,
                                    shrinkFactor: 0.5,
                                    growthFactor: 1 / 0.5,
                                    //minTimestepUnlessCantFulfillMax:1e-12,
                                    maximumAllowedError: 1,
                                    targetError: 0.01,
                                    allowBacktrackOnLessThanMinTimestep: false),
                                newtonRaphsonStoppingParameters: new MNANewtonRaphsonStoppingParameters(
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
                                ),
                                 cudaContextAssignedThreadPool: cudaContextAssignedThreadPool,
                                useGPU: true,
                                timestepSolutionAnalyser: new BasicTimestepSolutionAnalyser()
                            );
                        }
                        catch (Exception ex)
                        {
                            Logs.Default.Error(ex);
                            Logs.Default.Error("THERE WAS AN EXCEPTION");
                        }
                    });
                    useResults(model, resultsHandler, resultFileNameWithoutExtensionNormalized);
                });
            }
        }
        private static void PlotResults(
            Args args,
            DebugFlybackModel model, ResultsHandler resultsHandler,
            string resultNameNormalized)
        {
            if (resultsHandler.NResults < 1)
            {
                Console.Write("No results");
                return;
            }
            ResultRange rangeEnd = new ResultRange(ResultRangeType.NFromEnd, nEntries: 2000);
            ResultRange range = new ResultRange(ResultRangeType.All, nEntries: 6000);
            ResultsCsvHelper.CreateResultsCsv(Path.Combine(args.ResultsDirectory, "resultsEnd.csv"), model.Circuit,
                resultsHandler, rangeEnd);
            ResultsCsvHelper.CreateResultsCsv(Path.Combine(args.ResultsDirectory, "results.csv"), model.Circuit,
                resultsHandler, range);
            string errorRegionsDirectoryPath = Path.Combine(args.ResultsDirectory, "ErrorRegions");
            ResultsCsvHelper.CreateCsvFilesForHighErrorRegions(model.Circuit, resultsHandler, errorRegionsDirectoryPath);
            Func<string, string> getPlotFilePath =
                (suffix) => Path.Combine(args.ResultsDirectory, resultNameNormalized,
                suffix);
            Plot.Create(resultsHandler,
                getPlotFilePath("duty_cycle"),
                range,
                model.BangBangFlybackDriver.DutyCycle);
            Plot.Create(resultsHandler,
                getPlotFilePath("driver_resistance"),
                range,
                model.BangBangFlybackDriver.Resistance);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_voltage"),
                range,
                model.SupplyVoltageSource.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_current"),
                range,
                model.SupplyVoltageSource.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_power"),
                range,
                model.SupplyVoltageSource.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("flyback_primary_voltages"),
                range,
                model.NFlybackPrimary1);
            Plot.Create(resultsHandler,
                getPlotFilePath("flyback_secondary_voltages"),
                range,
                model.NFlybackSecondary2);
            Plot.Create(resultsHandler,
                getPlotFilePath("output_node_voltage"),
                range,
                model.NOutput);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_voltage"),
                range,
                model.RLoad.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_current"),
                range,
                model.RLoad.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_power"),
                range,
                model.RLoad.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_current"),
                range,
                model.Diode.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_voltage"),
                range,
                model.Diode.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_power"),
                range,
                model.Diode.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_current_end"),
                range,
                model.Diode.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_voltage_end"),
                range,
                model.Diode.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_power_end"),
                range,
                model.Diode.Power);

            return;
        }
        private static void CreateCsvFilesForHighErrorRegions(Circuit circuit, ResultsHandler resultsHandler,
            string directory)
        {
            IEnumerable<ResultsIndexRange> resultsIndexRangesExceedingDesireableMax = circuit.SimulationState
                .FulfilledDesiredMaxAll.GetIndexRangesForCondition(resultsHandler,
                v => v == 0, padding: 10);
            int index = 0;
            Directory.CreateDirectory(directory);
            foreach (ResultsIndexRange range in resultsIndexRangesExceedingDesireableMax)
            {
                string filePath = Path.Combine(directory, $"error_region_{index++}.csv");
                ResultsCsvHelper.CreateResultsCsv(filePath, circuit, resultsHandler, range);
            }
        }
        private static void HandleProgress(string solutionName, Action<StandardProgressHandler> callback)
        {

            StandardProgressHandler progressHandler = new StandardProgressHandler();
            using (progressHandler.RegisterPrintPercentSameLineWithEstimatedCompletionTime(
                $"Solving {solutionName}: "))
            {
                callback(progressHandler);
            }
        }
        private static void UsingResultsHandler(
            Args args, string resultNameWithoutExtension, DebugFlybackModel model,
            Action<ResultsHandler> callback)
        {

            string resultsFilePath = Path.Combine(args.ResultsDirectory,
                $"{resultNameWithoutExtension}.bin");
            Console.WriteLine($"Writing results to: \"{resultsFilePath}\"");
            using (OnDiskResultsHandler resultsHandler = new OnDiskResultsHandler(
                resultsFilePath,
              model.Circuit))
            {
                callback(resultsHandler);
            }
        }
*/




















/*using Core.Pool;
using ModifiedNodalAnalysis.Results;
using ModifiedNodalAnalysis.Solver;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Plotting;
using ModifiedNodalAnalysis.Interfaces;
using Args = ModifiedNodalAnalysis.FullCircuitSimulationArguments;
using Core.FileSystem;
using Core.Maths.Tolerances;
using Core.Arguments;
using Core.Maths.CUBLAS;
using Logging;
using ModifiedNodalAnalysis.Delegates;
namespace VoltageMultiplier.CircuitSimulation
{
    public class DebugFlybackSimulation
    {
        private delegate void DelegateUseResults(DebugFlybackModel model,
            ResultsHandler resultsHandler, string resultNameNormalized);
        public static void Debug(
            Args args,
            double maximumPowerOutputDesired,
            double minimumOutputVoltageDesired,
            double maximumOutputVoltageDesired
            )
        {
            double desiredOutputVoltage = maximumOutputVoltageDesired;
            double desiredPowerOutput = maximumPowerOutputDesired;
            Solve("debug flyback",
                args,
                desiredPowerOutput,
                desiredOutputVoltage,
                (model, resultsHandler, resultNameNormalized)
                    =>PlotResults(args, model, resultsHandler, resultNameNormalized),
                (model) =>
                {
                    return new IStoppingCondition[] {
                        new StopAtTime(1d/args.Frequency)
                    };
                });
        }
        private static void Solve(
        string solutionName,
        Args args,
        double desiredPowerOutput,
        double desiredOutputVoltage,
        DelegateUseResults useResults,
        Func<DebugFlybackModel, IStoppingCondition[]> createStoppingConditions
        )
        {

            double currentOutput = desiredPowerOutput / desiredOutputVoltage;
            double loadResistance = desiredOutputVoltage / currentOutput;
            DebugFlybackModel model = new DebugFlybackModel(
                args);
            IStoppingCondition[] stoppingConditions = createStoppingConditions(model);
            if (!stoppingConditions.Any())
                throw new ArgumentException($"{nameof(stoppingConditions)} had a length of zero. You must provide stopping conditions");
            IStoppingCondition stoppingCondition = stoppingConditions.Length > 1
                ? new CompositeStoppingCondition(stoppingConditions, stoppingConditions.Take(1).ToArray()) : stoppingConditions[0];
            string resultFileNameWithoutExtensionNormalized = FileHelper.NormalizeFileNameLowerCaseUnderscore(solutionName);
            using (CudaContextAssignedThreadPool cudaContextAssignedThreadPool = 
                new CudaContextAssignedThreadPool(1))
            {
                UsingResultsHandler(args,
                    resultFileNameWithoutExtensionNormalized,
                    model,
                    (resultsHandler) =>
                {
                    HandleProgress(solutionName, (progressHandler) =>
                    {
                        try
                        {
                            MNASolver.SolveOverTimeDomain(
                                model.Circuit,
                                stoppingCondition,
                                resultsHandler,
                                //0.000156208499998507,//
                                startTime: args.UseCachedResults||true ? resultsHandler.EndTimeInclusive : 0,
                                progressHandler: progressHandler,
                                timestepController: ,
                                newtonRaphsonStoppingParameters: new MNANewtonRaphsonStoppingParameters(
                                    maximumRelativeTolerance: 10,
                                    desiredMaximumRelativeTolerance: 0.1,
                                    targetRelativeTolerance: 0.01,
                                    relativeToleranceWeight: 1,
                                    absoluteToleranceVoltage: new AbsoluteTolerance(
                                            targetTolerance: 1e-2,
                                            desiredMaxTolerance: 0.1,
                                            maxTolerance: 10,
                                            weight: 1
                                        ),
                                    absoluteToleranceCurrent: new AbsoluteTolerance(
                                            targetTolerance: 1e-2,
                                            desiredMaxTolerance: 1,
                                            maxTolerance: 10,
                                            weight: 1
                                        ),
                                    maximumResidualTolerance: 1000,
                                    desiredMaximumResidualTolerance: 1,
                                    targetResidualTolerance: 0.01,
                                    residualToleranceWeight: 1,
                                    maxConvergenceSteps: 1000,
                                    failIfExceedsMaxConvergenceSteps: true
                                ),
                                 cudaContextAssignedThreadPool: cudaContextAssignedThreadPool,
                                useGPU: true, 
                                timestepSolutionAnalyser:new BasicTimestepSolutionAnalyser()
                            );
                        }
                        catch (Exception ex)
                        {
                            Logs.Default.Error(ex);
                            Logs.Default.Error("THERE WAS AN EXCEPTION");
                        }
                    });
                    useResults(model, resultsHandler, resultFileNameWithoutExtensionNormalized);
                });
            }
        }
        private static void PlotResults(
            Args args,
            DebugFlybackModel model, ResultsHandler resultsHandler,
            string resultNameNormalized)
        {
            if (resultsHandler.NResults < 1)
            {
                Console.Write("No results");
                return;
            }
            ResultRange rangeEnd = new ResultRange(ResultRangeType.NFromEnd, nEntries: 2000);
            ResultRange range = new ResultRange(ResultRangeType.All, nEntries:6000);
            ResultsCsvHelper.CreateResultsCsv(Path.Combine(args.ResultsDirectory, "resultsEnd.csv"), model.Circuit,
                resultsHandler, rangeEnd);
            ResultsCsvHelper.CreateResultsCsv(Path.Combine(args.ResultsDirectory, "results.csv"), model.Circuit,
                resultsHandler, range);
            string errorRegionsDirectoryPath = Path.Combine(args.ResultsDirectory, "ErrorRegions");
            CreateCsvFilesForHighErrorRegions(model.Circuit, resultsHandler, errorRegionsDirectoryPath);
            Func<string, string> getPlotFilePath =
                (suffix) => Path.Combine(args.ResultsDirectory, resultNameNormalized,
                suffix);
            Plot.Create(resultsHandler,
                getPlotFilePath("duty_cycle"),
                range,
                model.BangBangFlybackDriver.DutyCycle);
            Plot.Create(resultsHandler,
                getPlotFilePath("driver_resistance"),
                range,
                model.BangBangFlybackDriver.Resistance);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_voltage"),
                range,
                model.SupplyVoltageSource.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_current"),
                range,
                model.SupplyVoltageSource.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("voltage_source_power"),
                range,
                model.SupplyVoltageSource.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("flyback_primary_voltages"),
                range,
                model.NFlybackPrimary1);
            Plot.Create(resultsHandler,
                getPlotFilePath("flyback_secondary_voltages"),
                range,
                model.NFlybackSecondary2);
            Plot.Create(resultsHandler,
                getPlotFilePath("output_node_voltage"),
                range,
                model.NOutput);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_voltage"),
                range,
                model.RLoad.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_current"),
                range,
                model.RLoad.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("r_load_power"),
                range,
                model.RLoad.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_current"),
                range,
                model.Diode.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_voltage"),
                range,
                model.Diode.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_power"),
                range,
                model.Diode.Power);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_current_end"),
                range,
                model.Diode.Current);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_voltage_end"),
                range,
                model.Diode.Voltage);
            Plot.Create(resultsHandler,
                getPlotFilePath("diode_power_end"),
                range,
                model.Diode.Power);

            return;
        }
        private static void CreateCsvFilesForHighErrorRegions(Circuit circuit, ResultsHandler resultsHandler,
            string directory) {
            IEnumerable<ResultsIndexRange> resultsIndexRangesExceedingDesireableMax = circuit.SimulationState
                .FulfilledDesiredMaxAll.GetIndexRangesForCondition(resultsHandler,
                v => v ==0, padding: 10);
            int index = 0;
            Directory.CreateDirectory(directory);
            foreach (ResultsIndexRange range in resultsIndexRangesExceedingDesireableMax)
            {
                string filePath = Path.Combine(directory, $"error_region_{index++}.csv");
                ResultsCsvHelper.CreateResultsCsv(filePath, circuit, resultsHandler, range);
            }
        }
        private static void HandleProgress(string solutionName, Action<StandardProgressHandler> callback)
        {

            StandardProgressHandler progressHandler = new StandardProgressHandler();
            using (progressHandler.RegisterPrintPercentSameLineWithEstimatedCompletionTime(
                $"Solving {solutionName}: "))
            {
                callback(progressHandler);
            }
        }
        private static void UsingResultsHandler(
            Args args, string resultNameWithoutExtension, DebugFlybackModel model,
            Action<ResultsHandler> callback)
        {

            string resultsFilePath = Path.Combine(args.ResultsDirectory,
                $"{resultNameWithoutExtension}.bin");
            Console.WriteLine($"Writing results to: \"{resultsFilePath}\"");
            using (OnDiskResultsHandler resultsHandler = new OnDiskResultsHandler(
                resultsFilePath,
              model.Circuit))
            {
                callback(resultsHandler);
            }
        }
    }
}*/