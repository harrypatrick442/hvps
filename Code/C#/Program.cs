using CircuitAnalysis;
using CircuitAnalysis.ComponentSpecs;
using InfernoDispatcher;
using Core.MemoryManagement;
using Shutdown;
using Logging;
using VoltageMultiplier.FlybackTransformerModelling;
using ModifiedNodalAnalysis.Results;
using ModifiedNodalAnalysis.Plotting;
using VoltageMultiplier.CircuitSimulation;
using ModifiedNodalAnalysis;
using DD1800 = CircuitAnalysis.DD1800;
using ModifiedNodalAnalysis.Solver;
using Core.Maths.CUBLAS;
using Core.FileSystem;
using VoltageMultiplier.CircuitSimulation.Simulations;
using Core.ThisSystem;

using Paths = VoltageMultiplier.FlybackTransformerModelling.Paths;
using Core;
using Core.Maths.Tolerances;
using VoltageMultiplier.CircuitSimulation.Args;
using Core.Arguments;
namespace VoltageMultiplier
{
    // Example usage:
    public class Program
    {
        /*
         * https://wias-berlin.de/software/tetgen/fformats.html
         * */
        private const double MAXIMUM_POWER_OUTPUT_DESIRED = 200,
            MAXIMUM_OUTPUT_VOLTAGE_DESIRED = 1000000,
            MINIMUM_OUTPUT_VOLTAGE_DESIRED = 100000,
            VOLTAGE_PER_VILLARD_CASCADING_VOLTAGE_MULTIPLIER_STAGE = 13000,
            FLYBACK_SECONDARY_VOLTAGE = 15000,
            SWITCHING_FREQUENCY = 16000,
            MINIMUM_INPUT_VOLTAGE = 36,
            MAXIMUM_INPUT_VOLTAGE = 36,
            FLYBACK_TRANSFORMER_EFFICIENCY = 1,
            MINIMUM_DUTY_CYCLE = 0.0,
            MAXIMUM_DUTY_CYCLE = 0.5,
            T_AMBIENT = 300,
            SAFETY_FACTOR = 1.2,
            GATE_DRIVER_SUPPLY_VOLTAGE= 12,
            MOSFET_SWITCHING_TIME_AS_PROPORTION_SWITCHING_PERIOD = 0.001d,
            FIRST_STAGE_VOLTAGE_FEEDBACK_MAX = FLYBACK_SECONDARY_VOLTAGE * SAFETY_FACTOR,
            FIRST_STAGE_VOLTAGE_FEEDBACK_MIN = 1000,
            FIRST_STAGE_FEEDBACK_MAX_POWER_CONSUMPTION = 1;
        private const int 
            N_BUNDLES_SECONDARY_WNDING = 12,
            PRIMARY_WIRE_GUAGE =20,
            SECONDARY_WIRE_GUAGE = 30,
            N_MULTIPLIER_STAGES = 1,
            N_CUDA_CONTEXT = 10,
            FIRST_STAGE_FEEDBACK_N_INTERVALS_MIN = 40,
            MINIMUM_CYCLES_TO_REACH_FULL_CAPACITOR_VOLTAGE_DESIRED = 6;

        private const string TEMP_DIRECTORY_PATH = "D:\\temp\\";
        private static readonly CoreSpecs CORE_SPECS = new PC40_UY30_MnZn_U();
        private const bool HAS_FLUX_MEASUREMENT_BOUNDARY_WINDING = true;
        private static readonly DiodeSpecs VILLARD_DIODE_SPECS = new DD1800();
        private static readonly BobinSpecs BOBIN_SPECS = new CustomBobin();
        private static readonly WireSpecs PRIMARY_WIRE_SPECS = new EnameledCopperWire(PRIMARY_WIRE_GUAGE);
        private static readonly WireSpecs SECONDARY_WIRE_SPECS = new EnameledCopperWire(SECONDARY_WIRE_GUAGE);
        private static readonly CapacitorSpecs VILLARD_CAPACITOR_SPECS = new HVCC203Y6P102MEAX();
        private static readonly ResistorSpecs VILLARD_CAPACITOR_DISCHARGE_RESISTOR_SPECS = 
            new CompositeSeriesResistor(new MHR0422SA_1G(), 2);
        private static readonly ResistorSpecs VILLARD_OUTPUT_CURRENT_LIMITING_RESISTOR =
            new CompositeSeriesResistor(new HHV3SSJT_73_20K(), 200);//20k ohms per resistor. 200 resistors. Total 4000000 ohms.
        private static readonly ResistorSpecs SECONDARY_CURRENT_SENSE_RESISTOR = new Ohmite13FR200E();
#region Driver
        private static readonly MosfetSpecs DRIVER_MOSEFET_SPECS = new CSD19534KCS();
        private static readonly GateDriverSpecs DRIVER_GATE_DRIVER_SPECS = new TC1413N();
#endregion
        #region First Stage Voltage Feedback
        private static readonly ZenerDiodeSpecs FIRST_STAGE_FEEDBACK_SIGNAL_ZENER_CLAMP = new _1N4619();
        //30147
        private static readonly ResistorSpecs FIRST_STAGE_FEEDBACK_LOW_SIDE_RESISTOR = new MFR_25FBF52_30K1();
        private static readonly ResistorSpecs FIRST_STAGE_FEEDBACK_HIGH_SIDE_RESISTOR = new CompositeSeriesResistor(
            new SM108035005FE(), 4);
        private static readonly CapacitorSpecs FIRST_STAGE_FEEDBACK_LOW_SIDE_CAPACITOR = new C322C102F5G5TA();
        #endregion


        private static PIControllerConstants PI_CONTROLLER_CONSTANTS = new PIControllerConstants(
                pSecondaryCurrent:1,
                iSecondaryCurrent:1,
                pFirstStageVoltage:1,
                iFirstStageVoltage:1,
                pOutputVoltage:1,
                iOutputVoltage:1
            );
        public static void Main(string[] args)
        {
            Logs.Initialize(LogFilePathDefault.Value);
            ShutdownManager.Initialize(Environment.Exit, () => Logs.Default);
            GpuMemoryInfoNVML.Initialize();
            CudaContextAssignedThreadPool? mnaCudaThreadPool = null;
            try
            {
                mnaCudaThreadPool = new CudaContextAssignedThreadPool(N_CUDA_CONTEXT);
                MNAInversionHandler.Initialize(mnaCudaThreadPool);
                Dispatcher.InitializeWithNative(Console.WriteLine);
                Func<bool> enteredYes = () =>
                {
                    bool v = (Console.ReadKey().KeyChar + "").ToLower() != "n";
                    Console.WriteLine();
                    return v;
                };
                Console.WriteLine("Used cached flyback transformer FEA results? (y/n) [y]:");
                bool useCachedFlybackTransformerFEAResults = false;// enteredYes();
                Console.WriteLine("Used cached circuit simulation results? (y/n) [y]:");
                bool useCachedCircuitSimulationResults = false;// enteredYes();
                Console.WriteLine();
                var paths = new Paths(TEMP_DIRECTORY_PATH);
                double averageSecondaryCurrent = MAXIMUM_POWER_OUTPUT_DESIRED / FLYBACK_SECONDARY_VOLTAGE;
                DiodeDesignEquations.CalculatePTotalAndOperatingTemperature(
                    VILLARD_DIODE_SPECS,
                    averageSecondaryCurrent,
                    0,
                    MAXIMUM_DUTY_CYCLE,
                    VOLTAGE_PER_VILLARD_CASCADING_VOLTAGE_MULTIPLIER_STAGE, SWITCHING_FREQUENCY,
                    T_AMBIENT,
                    SAFETY_FACTOR,
                    out double pTotalPerDiode,
                    out double diodeOperatingTemperature);
                BasicCalculationsFlybackSpecs basicCalculationsFlybackSpecs = FlybackTransformerModelling
                    .Modeler.Model(
                    HAS_FLUX_MEASUREMENT_BOUNDARY_WINDING,
                    CORE_SPECS,
                    BOBIN_SPECS,
                    PRIMARY_WIRE_SPECS,
                    SECONDARY_WIRE_SPECS,
                    SWITCHING_FREQUENCY,
                    MAXIMUM_DUTY_CYCLE,
                    MINIMUM_INPUT_VOLTAGE,
                    MAXIMUM_INPUT_VOLTAGE,
                    MAXIMUM_POWER_OUTPUT_DESIRED,
                    FLYBACK_SECONDARY_VOLTAGE,
                    FLYBACK_TRANSFORMER_EFFICIENCY,
                    useCachedFlybackTransformerFEAResults,
                    //N_BUNDLES_SECONDARY_WNDING,
                    File.ReadAllBytes("C:\\repos\\snippets\\CircuitAnalysis\\VoltageMultiplier\\Meshes\\UsingUY30MnZnPC40130x95mm.obj"),
                    new VolumeConstraints(
                        windingElement: 1e-8,//1e-9,
                        coreElement: 5e-8,//5e-8,//8,
                        freeSpaceElement: 5e-7,//5e-8,
                        airGapElement: 3e-10//3e-10
                    ),
                    2,
                    new Conductivities(),
                    paths,
                    out FEMResultFlybackSpecs femResultFlybackSpecs
                );
                double capacitorEnergyMax = VILLARD_CAPACITOR_SPECS.Capacitance * Math.Pow(VOLTAGE_PER_VILLARD_CASCADING_VOLTAGE_MULTIPLIER_STAGE, 2) / 2d;
                int nCyclesBeforeRiskingOverdrivingCapacitorVoltage = (int)Math.Floor(capacitorEnergyMax / basicCalculationsFlybackSpecs.MaximumEnergyPerCycle);
                if (nCyclesBeforeRiskingOverdrivingCapacitorVoltage < MINIMUM_CYCLES_TO_REACH_FULL_CAPACITOR_VOLTAGE_DESIRED) {
                    throw new Exception("Energy per flyback cycle is too much relative to capacitor energy." +
                        " Risk over overdriving capacitors." +
                        $" {nCyclesBeforeRiskingOverdrivingCapacitorVoltage} cycles could safely happen" +
                        " before risking exceeding capacitor voltage. Desired value was: " +
                        $"{MINIMUM_CYCLES_TO_REACH_FULL_CAPACITOR_VOLTAGE_DESIRED}");
                }
                    if (basicCalculationsFlybackSpecs.I_s_peak > VILLARD_DIODE_SPECS.I_FRM * SAFETY_FACTOR)
                {
                    throw new Exception("DiodeBase {nameof(VILLARD_DIODE_SPECS.I_FRM)}"
                        + (
                            basicCalculationsFlybackSpecs.I_s_peak > VILLARD_DIODE_SPECS.I_FRM
                            ? " if safety factor included."
                            : "."
                         )
                    );
                }
                FlybackDriverModeller.Model(
                    mosfetSpecs:DRIVER_MOSEFET_SPECS,
                    gateDriverSpecs:DRIVER_GATE_DRIVER_SPECS,
                    Vdsmax:MAXIMUM_INPUT_VOLTAGE,
                    Vdsmin:MINIMUM_INPUT_VOLTAGE,
                    gateDriverSupplyVoltage:GATE_DRIVER_SUPPLY_VOLTAGE,
                    IdMax:basicCalculationsFlybackSpecs.I_p_peak,
                    switchingFrequency:SWITCHING_FREQUENCY,
                    desiredSwitchingTime:(1d/SWITCHING_FREQUENCY)*MOSFET_SWITCHING_TIME_AS_PROPORTION_SWITCHING_PERIOD,
                    safetyFactor:SAFETY_FACTOR
                );
                double slewRateForSingleStage = basicCalculationsFlybackSpecs.I_s_peak / VILLARD_CAPACITOR_SPECS.Capacitance;
                //30147
                HighVoltageDividerFeedbackEquations.Design(
                    FIRST_STAGE_FEEDBACK_SIGNAL_ZENER_CLAMP,
                    signalVoltageClampRequired:3.3,
                    maximumOperatingSignalVoltage:2.7,
                    measuredVoltageMax: FIRST_STAGE_VOLTAGE_FEEDBACK_MAX,
                    measuredVoltageMin:FIRST_STAGE_VOLTAGE_FEEDBACK_MIN,
                    nIntervalsMin: FIRST_STAGE_FEEDBACK_N_INTERVALS_MIN,
                    powerConsumptionMax:FIRST_STAGE_FEEDBACK_MAX_POWER_CONSUMPTION,
                    maximumSlewRateMeasuredVoltage: slewRateForSingleStage,
                    safetyFactor:SAFETY_FACTOR,
                    highSideResistor:FIRST_STAGE_FEEDBACK_HIGH_SIDE_RESISTOR,
                    lowSideResistor: FIRST_STAGE_FEEDBACK_LOW_SIDE_RESISTOR
                );
                RCDSnubberSpecs snubberParametersApproximateNotInUse = RCDSnubberEquations.SizeSnubber(
                    L_leak: femResultFlybackSpecs.L_ks / Math.Pow(basicCalculationsFlybackSpecs.NsOverNp, 2),
                    I_peak: basicCalculationsFlybackSpecs.I_p_peak,
                    V_clamp: MAXIMUM_INPUT_VOLTAGE,
                    f_s: SWITCHING_FREQUENCY);
                RCDSnubberSpecs snubberParametersApproximate = RCDSnubberEquations.SizeSnubber2(
                    L_lk1: femResultFlybackSpecs.L_ks / Math.Pow(basicCalculationsFlybackSpecs.NsOverNp, 2),
                    I_peak: basicCalculationsFlybackSpecs.I_p_peak,
                    n: basicCalculationsFlybackSpecs.NpOverNs,
                    V_o: FLYBACK_SECONDARY_VOLTAGE,
                    f_s: SWITCHING_FREQUENCY);
                Console.WriteLine("Approximate snubber parameters required:");
                snubberParametersApproximate.Print();



                var circuitSimulationArguments =
                    new FullCircuitSimulationArguments(
                    SAFETY_FACTOR,
                    SWITCHING_FREQUENCY,
                    V_in: MAXIMUM_INPUT_VOLTAGE,
                    D_min: MINIMUM_DUTY_CYCLE,
                    D_max: MAXIMUM_DUTY_CYCLE,
                    N_MULTIPLIER_STAGES,
                    basicCalculationsFlybackSpecs,
                    PI_CONTROLLER_CONSTANTS,
                    femResultFlybackSpecs,
                    VILLARD_CAPACITOR_DISCHARGE_RESISTOR_SPECS,
                    VILLARD_CAPACITOR_SPECS,
                    VILLARD_DIODE_SPECS,
                    VILLARD_OUTPUT_CURRENT_LIMITING_RESISTOR,
                    SECONDARY_CURRENT_SENSE_RESISTOR,
                    snubberSpecs: snubberParametersApproximate,
                    paths,
                    useCachedResults: useCachedCircuitSimulationResults
                );



                double flybackEnergySingleCycle = 0.5d * femResultFlybackSpecs.L_ms 
                    * Math.Pow(basicCalculationsFlybackSpecs.I_s_peak, 2);
                double voltageSingleStageOutputCanReachInSingleCycle
                    = Math.Sqrt(flybackEnergySingleCycle * 2 / VILLARD_CAPACITOR_SPECS.Capacitance);
                int nCyclesToChargeSingleStageToMaximumSafeVoltage = DetermineNCyclesToChargeSmallNumberOfOutputStages(circuitSimulationArguments);
                /*var firstStageFeedbackPotentialDividerFirstCycleSimulationResults 
                    = new HighVoltagePotentialDividerSimualtion(
                    voltageReached: voltageSingleStageOutputCanReachInSingleCycle,
                    duration: 1d / (2d * SWITCHING_FREQUENCY),
                    highSideResistor: FIRST_STAGE_FEEDBACK_HIGH_SIDE_RESISTOR,
                    lowSideResistor: FIRST_STAGE_FEEDBACK_LOW_SIDE_RESISTOR).Run(
                        solutionName: "first_stage_feedback_potential_divider_first_cycle_simulation",
                        rootResultsDirectory: paths.CircuitSimulationResultsDirectory,
                        useCachedResults: true,
                        new HighVoltagePotentialDividerSimulationArgs());
                double flybackEnergyNSafeCycles = flybackEnergySingleCycle * nCyclesToChargeSingleStageToMaximumSafeVoltage;

                double voltageSingleStageOutputCanReachForNSafeChargingCycles
                    = Math.Sqrt(flybackEnergyNSafeCycles * 2 / VILLARD_CAPACITOR_SPECS.Capacitance);
                var firstStageFeedbackPotentialDividerNCyclesToChargeSimulationResults
                    = new HighVoltagePotentialDividerSimualtion(
                    voltageReached: voltageSingleStageOutputCanReachForNSafeChargingCycles,
                    duration: (nCyclesToChargeSingleStageToMaximumSafeVoltage-0.5/*should have appropriate feedback before start next cycle
                    ) / SWITCHING_FREQUENCY,
                    highSideResistor: FIRST_STAGE_FEEDBACK_HIGH_SIDE_RESISTOR,
                    lowSideResistor: FIRST_STAGE_FEEDBACK_LOW_SIDE_RESISTOR).Run(
                        solutionName: "first_stage_feedback_potential_divider_n_cycles_to_charge_simulation",
                        rootResultsDirectory: paths.CircuitSimulationResultsDirectory,
                        useCachedResults: true,
                        new HighVoltagePotentialDividerSimulationArgs());
                */
                DetermineCyclesToChargeSmallNumbersOfOutputStages(
                    paths.CircuitSimulationResultsDirectory,
                    useCachedCircuitSimulationResults,
                    circuitSimulationArguments);
            }
            finally {
                mnaCudaThreadPool?.Dispose();
            }
        }
        private static int DetermineNCyclesToChargeSmallNumberOfOutputStages(
            FullCircuitSimulationArguments args) {
            double flybackEnergySingleCycle = 0.5d * args.L_ms * Math.Pow(args.MaximumFlybackSecondaryCurrent, 2);
            double approximateVoltageSingleStageFromSingleCycle
                = Math.Sqrt(flybackEnergySingleCycle * 2 / args.Capacitance);
            double proportionMaximumCapacitorVoltageReachedUnderIdealConditions
                = approximateVoltageSingleStageFromSingleCycle / args.MaximumSafeOperatingVillardCapacitorVoltage;
            double proportionMaximumDiodeVoltageReachedUnderIdealConditions
                = approximateVoltageSingleStageFromSingleCycle / args.MaximumSafeOperatingVillardDiodeVoltage;
            if (proportionMaximumCapacitorVoltageReachedUnderIdealConditions > 1)
            {
                throw new Exception($"Theoretical absolute voltage a single capacitor " +
                    $"output can reach from a single flyback cycle exceeded the safe limit for the capacitor. Voltage theoretically reachable was: {approximateVoltageSingleStageFromSingleCycle}. Maximum safe limit was: {args.MaximumSafeOperatingVillardCapacitorVoltage} ");
            }
            if (proportionMaximumDiodeVoltageReachedUnderIdealConditions > 1)
            {
                throw new Exception($"Theoretical absolute voltage a single capacitor " +
                    $"output can reach from a single flyback cycle exceeded the safe limit for the diode. Voltage theoretically reachable was: {approximateVoltageSingleStageFromSingleCycle}. Maximum safe limit was: {args.MaximumSafeOperatingVillardDiodeVoltage} ");
            }
            Console.WriteLine($"Theoretical absolute voltage a single capacitor can reach from a single flyback cycle was: {approximateVoltageSingleStageFromSingleCycle}. This is proportion: {proportionMaximumCapacitorVoltageReachedUnderIdealConditions} of the maximum safe limit for a capacitor: {args.MaximumSafeOperatingVillardCapacitorVoltage} and for a diode: {args.MaximumSafeOperatingVillardDiodeVoltage}.");
            double maximumSafeVoltage = args.MaximumSafeOperatingVillardCapacitorVoltage > args.MaximumSafeOperatingVillardDiodeVoltage
                ? args.MaximumSafeOperatingVillardCapacitorVoltage : args.MaximumSafeOperatingVillardDiodeVoltage;
            double energyAtMaximumVoltage = 0.5d * args.Capacitance * Math.Pow(maximumSafeVoltage, 2);
            double maximumAllowableEnergy = energyAtMaximumVoltage - flybackEnergySingleCycle;
            double maximumCapacitorVoltageCanContinueDrivingFlyback = Math.Sqrt(maximumAllowableEnergy * 2d / args.Capacitance);
            int nCyclesSafely = 1;
            while (true)
            {
                double capacitorVoltageForTotalEnergy =
                 Math.Sqrt(((nCyclesSafely + 1) * flybackEnergySingleCycle) * 2d / args.Capacitance);
                if (capacitorVoltageForTotalEnergy >= maximumSafeVoltage)
                {
                    break;
                }
                nCyclesSafely++;
            }
            Console.WriteLine($"This is safe. Up to {nCyclesSafely} transformer cycles can happen before the next cycle could exceed the safe limit.");
            Console.WriteLine($"This means for a bang bang driver the driver should stop driving when the capacitor voltage reaches {maximumCapacitorVoltageCanContinueDrivingFlyback} or less (include safety factor)");
            return nCyclesSafely;

        }
        private static void DetermineCyclesToChargeSmallNumbersOfOutputStages(
            string rootResultsDirectory,
            bool useCache, 
            FullCircuitSimulationArguments args) {
            NCyclesToChargeSmallNumberOutputStagesResults simulationResults = new NCyclesToChargeSmallNumberOutputStagesSimulation(
                1, null, nCycles: 5).Run(
                $"flyback_transformer_driver_1_villard_stages",
                rootResultsDirectory,
                useCache,
                args)!;
            
            double minCapacitorVoltageMagnitude = Math.Abs(simulationResults.MinFirstStageCapacitorVoltageReached);
            double maxCapacitorVoltageReached = minCapacitorVoltageMagnitude > simulationResults.MaxFirstStageCapacitorVoltageReached 
                ? minCapacitorVoltageMagnitude : simulationResults.MaxFirstStageCapacitorVoltageReached;
            if (maxCapacitorVoltageReached > args.MaximumSafeOperatingVillardCapacitorVoltage) {
                throw new Exception($"Simulation first stage capacitor reached voltage: {maxCapacitorVoltageReached} " +
                    $"exceeding safe operating voltage for capacitor: {args.MaximumSafeOperatingVillardCapacitorVoltage}");
            }
            Console.WriteLine($"Simulation first stage capacitor reached voltage: {maxCapacitorVoltageReached} " +
                    $"which is within the safe operating voltage for capacitor: {args.MaximumSafeOperatingVillardCapacitorVoltage}");
            
            double firstStageDiodeMinimumCurrentMagnitude = Math.Abs(simulationResults.FirstStageDiodeMinCurrent);
            double firstStageDiodeMaximumCurrent = simulationResults.FirstStageDiodeMaxCurrent > firstStageDiodeMinimumCurrentMagnitude
                ? simulationResults.FirstStageDiodeMaxCurrent : firstStageDiodeMinimumCurrentMagnitude;
            if (firstStageDiodeMaximumCurrent > args.MaximumSafeOperatingVillardDiodeCurrent)
            {
                throw new Exception($"Simulation first stage diode reached current: {firstStageDiodeMaximumCurrent} " +
                    $"exceeding safe operating voltage for diode: {args.MaximumSafeOperatingVillardDiodeCurrent}");
            }
            Console.WriteLine($"Simulation first stage diode reached current: {firstStageDiodeMaximumCurrent} " +
                    $"which is within the safe operating current for diode: {args.MaximumSafeOperatingVillardDiodeCurrent}");
            
            double firstStageDiodeMinimumVoltageMagnitude = Math.Abs(simulationResults.FirstStageDiodeMinVoltage);
            if (!simulationResults.FirstStageDiodeMaxVoltage.Within(0.05, args.MaximumSafeOperatingVillardDiodeVoltage))
            {
                 throw new Exception($"Simulation first stage diode reached voltage: {simulationResults.FirstStageDiodeMaxVoltage} " +
                    $"exceeding safe operating voltage for diode: {args.MaximumSafeOperatingVillardDiodeVoltage}");
            }
            Console.WriteLine($"Simulation first stage diode reached voltage: {simulationResults.FirstStageDiodeMaxVoltage} " +
                    $"which is within the safe operating voltage for diode: {args.MaximumSafeOperatingVillardDiodeVoltage}");
            NCyclesToChargeSmallNumberOutputStagesResults simulationResultsTwoStages = 
                new NCyclesToChargeSmallNumberOutputStagesSimulation(
                1, null, nCycles:2).Run(
                $"flyback_transformer_driver_1_villard_stages_2_cycles",
                rootResultsDirectory,
                useCache,
                args)!;
        }
        /*
        /// <summary>
        /// WRONG APPROACH. RESISTOR IS SUPERIOR.
        /// </summary>
        /// <param name="diodeSpecs"></param>
        /// <param name="maximumSteadyStateSecondaryCurrent"></param>
        /// <returns></returns>
        /// <exception cref="Exception"></exception>
        private static double DetermineOutputInductorValue(DiodeSpecs diodeSpecs, double maximumSteadyStateSecondaryCurrent)
        {
            double desiredMaximumCurrent = (diodeSpecs.I_FRM / SAFETY_FACTOR)- maximumSteadyStateSecondaryCurrent;
            if (desiredMaximumCurrent<=0) {
                throw new Exception("The maximum secondary current was far too high");
            }
            double currentMaxInductance = 1e9d;
            double currentMinInductance = 1e-9;
            Inductor outputInductor;
            InMemoryResultsHandler resultsHandler;
            double resonantFrequency;
            double inductance;
            double maximumCurrent;
            NonGroundNode n1;
            while (true)
            {
                inductance = (currentMaxInductance+currentMinInductance)/2d;
                double totalSeriesCapacitance = 2 * VOLTAGE_MULTIPLIER_CAPACITOR_FARADS
                    * OUTPUT_VOLTAGE_DESIRED / FLYBACK_SECONDARY_VOLTAGE;
                var nodes = new NodesCollection();
                var components = new ComponentsCollection();
                var ground = new Ground(nodes);
                n1 = new NonGroundNode("n1", nodes);
                Capacitor totalSeriesCapacitor = new Capacitor(
                    "c_series",
                    totalSeriesCapacitance,
                    ground,
                    n1,
                    components
                );
                outputInductor = new Inductor(
                    "l_out",
                    inductance,
                    n1,
                    ground,
                    components);
                outputInductor.Unknown = maximumSteadyStateSecondaryCurrent;
                n1.Voltage = OUTPUT_VOLTAGE_DESIRED;
                resonantFrequency = 1d / (2d * Math.PI * Math.Sqrt(inductance * totalSeriesCapacitance));
                double timestep = 1d / (resonantFrequency * 100000d),
                    duration = 1d / resonantFrequency;
                resultsHandler = new InMemoryResultsHandler(
                    timestep, duration);
                ModifiedNodalAnalysisSolver.SolveOverTimeDomain(nodes,
                    components,
                    timestep,
                    duration,
                    resultsHandler,
                    populateUnknownsFromComponents: true
               );
                maximumCurrent = GetMaximumCurrentAbs(resultsHandler, totalSeriesCapacitor, timestep);
                if (maximumCurrent > desiredMaximumCurrent)
                {
                    currentMinInductance = inductance;
                }
                else { 
                    currentMaxInductance = inductance;
                    if ((desiredMaximumCurrent - maximumCurrent) / desiredMaximumCurrent < 1e-3) {
                        break;
                    }
                }
            }
           var plot = new Plot(resultsHandler);
            plot.Add(outputInductor);
            plot.Save("C:\\repos\\snippets\\CircuitAnalysis\\inductorCalculation.png");
            Console.WriteLine($"Picked an output inductance of {inductance} Henries to allow a maximum shorted output current of {maximumCurrent} Amps.");
            Console.WriteLine($"Resonant frequency of output inductor in series with voltage multiplier capacitance is: {resonantFrequency} Hz");
            return 0;
        }
        private static double GetMaximumCurrentAbs(
            ResultsHandler resultsHandler, Capacitor capacitor,
            double timestep) {
            NodeBase[] capacitorNonGroundNodes = capacitor.Nodes.Where(n => n.IsNotGround).ToArray();
            NodeBase capacitorNodeA = capacitorNonGroundNodes[0];
            NodeBase? capacitorNodeB = capacitorNonGroundNodes.Length > 1 ? capacitorNonGroundNodes[1] : null;
            Func<double[], double> getCapacitorVolageNodeB = capacitorNodeB == null ? (buf) => 0 : (buf) => buf[capacitorNodeB.Index];
            double maximumCurrent = 0;
            var buffer = resultsHandler.Buffer.ToArray();
            for (int index = 0; index < buffer.Length - 1; index++)
            {
                double[] thisResult = buffer[index];
                double[] nextResult = buffer[index+1];
                double capacitorVoltageThisResult = thisResult[capacitorNodeA.Index] - getCapacitorVolageNodeB (thisResult);
                double capacitorVoltageNextResult = nextResult[capacitorNodeA.Index] - getCapacitorVolageNodeB(nextResult);
                double dv = (capacitorVoltageNextResult - capacitorVoltageThisResult);
                double I = capacitor.Farads * dv / timestep;
                    if (I > maximumCurrent)
                        maximumCurrent = I;
                
            }
            return maximumCurrent;
        }*/
    }
}

/*solveResult.SaveUnknownsVector(Path.Combine(outputDirectory, "magneticVectorPotentials.bin"));
var meshMagneticVectorPotentials = new MeshMagneticVectorPotentials(magneticFieldMesh);
ContourPlotHelper.Plot(firstHalfWindingMesh,
new FinitePlane(
new Vector3D(0, 0, 0.018),
new Vector3D(1, 0, 0),
new Vector2D(0.2, 0.2)),
Path.Combine(outputDirectory, $"voltage_{i}.png"), 200);
QuiverPlotHelper.Plot(
new FinitePlane(
new Vector3D(0, 0, 0.018),
new Vector3D(1, 0, 0),
new Vector2D(0.2, 0.2)),
meshMagneticVectorPotentials.GetMagneticVectorPotentialAtPoint,
40, 2000,
Path.Combine(outputDirectory, $"magneticField_X.png")
);

QuiverPlotHelper.Plot(
new FinitePlane(
new Vector3D(0, 0, 0.018),
new Vector3D(0, 1, 0),
new Vector2D(0.2, 0.02)),
meshMagneticVectorPotentials.GetMagneticVectorPotentialAtPoint,
40, 2000,
Path.Combine(outputDirectory, $"magneticField_Y.png")
);
QuiverPlotHelper.Plot(
new FinitePlane(
new Vector3D(0, 0, 0),
new Vector3D(0, 0, 1),
new Vector2D(0.2, 0.02)),
meshMagneticVectorPotentials.GetMagneticVectorPotentialAtPoint,
40, 2000,
Path.Combine(outputDirectory, $"magneticField_Z.png")
);
}
//DirectoryHelper.CopyRecurively(temporaryDirectory.AbsolutePath, outputDirectory);
*/


/*
                        var gs = firstHalfWindingMesh.Faces.GroupBy(n => n.Boundary).Select(g => g.ToArray()).ToArray();
                        var fixedVoltageNodes = gs[2].SelectMany(f => f.Nodes).GroupBy(n => n).Select(g => g.First()).ToHashSet();
                        var fixedCurrentDensityNodes = gs[1].SelectMany(f => f.Nodes).GroupBy(n => n).Select(g => g.First()).ToHashSet();
                        var nonBoundaryNodes = firstHalfWindingMesh.Nodes.Where(n => !fixedVoltageNodes.Contains(n) && !fixedCurrentDensityNodes.Contains(n)).ToArray();
                        var getCurrentRange = (IEnumerable<Node> nodes) =>
                        {
                            double min = nodes.First().Value;
                            double max = min;
                            foreach (var node in nodes.Skip(1))
                            {
                                if (node.Value < min)
                                    min = node.Value;
                                else if (node.Value > max)
                                    max = node.Value;
                            }
                            return new double[] { min, max };
                        };
                        var rangeFixedVoltage = getCurrentRange(fixedVoltageNodes);
                        var rangeFixedCurrent = getCurrentRange(fixedCurrentDensityNodes);
                        var rangeOtherNodes = getCurrentRange(nonBoundaryNodes);*/