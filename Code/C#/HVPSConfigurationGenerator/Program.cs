using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using HVPSConstants;
using ConfigurationClassBuilder;
using FPGAConstantsGenerator;
using HVPSCore.Enums;
namespace HVPSConfigurationGenerator
{
    class Program
    {

        static void Main(string[] args)
        {
            string reposDirectory = Assembly.GetEntryAssembly()!.Location;
            while (Path.GetFileName(reposDirectory).ToLower() != "repos")
            {
                reposDirectory = Directory.GetParent(reposDirectory)!.FullName;
            }
            if (Constants.VillardCapacitorDeratedMaximumVoltage > Constants.VillardCapacitorVoltageRating / Constants.SafetyFactor)
            {
                throw new Exception($"The value of {nameof(Constants)}.{nameof(Constants.VillardCapacitorDeratedMaximumVoltage)} cannot be greater than {nameof(Constants)}.{nameof(Constants.VillardCapacitorVoltageRating)}/{nameof(Constants)}.{nameof(Constants.SafetyFactor)}");
            }
            double maxFlybackEnergyPerCycle = 0.5 * Constants.FlybackTransformerMeasuredPrimaryInductances * Math.Pow(Constants.FlybackTransformerMaximumCurrent, 2);
            double flybackCapacitorMinimumCapacitanceForTolerance = Constants.VillardCapacitorCapacitance * (100.0 - Constants.VillardCapacitorTolerancePercent) / 100.0;
            double lowestCapacitorEnergyAtMaximumCapacitorVoltage =
                0.5 * flybackCapacitorMinimumCapacitanceForTolerance
                * Math.Pow(Constants.VillardCapacitorDeratedMaximumVoltage, 2);
            double maximumAllowableCapacitorEnergy = lowestCapacitorEnergyAtMaximumCapacitorVoltage - maxFlybackEnergyPerCycle;
            //Subtract the energy of one cycle because voltage might land over the threshold by up to one cycles worth of energy
            double firstStageVoltageThreshold = Math.Pow(maximumAllowableCapacitorEnergy / (0.5 * flybackCapacitorMinimumCapacitanceForTolerance), 0.5);
            //NOTE we ignore later stages for this and assume all energy is dumped into the first capacitor pair. The measurement actually comes from the first stage second capacitor voltage but this is a pretty safe estimate.
            double maximumAchievableOutputVoltage = (Constants.NStages/*Not 2* because of how is driven*/ * (firstStageVoltageThreshold - Constants.MaximumVillardDiodeForwardVoltageDrop));
            if (maximumAchievableOutputVoltage < Constants.MaximumDesiredOutputVoltage)
            {
                throw new Exception($"The calculated maximum achievable o0utput voltage for {Constants.NStages} is {maximumAchievableOutputVoltage} which is less than the desired maximum output voltage of {Constants.MaximumDesiredOutputVoltage}");
            }
            double proportionFirstStageVoltageThresholdForMaximumOutputVoltage =
                ((Constants.MaximumDesiredOutputVoltage + (Constants.NStages * Constants.MaximumVillardDiodeForwardVoltageDrop))) / maximumAchievableOutputVoltage;
            if (proportionFirstStageVoltageThresholdForMaximumOutputVoltage > 1)
            {
                throw new Exception($"Will not be able to achieve maximum desirable output voltage of {Constants.MaximumDesiredOutputVoltage}");
            }
            double maxOutputVoltageThreshold = Constants.MaximumDesiredOutputVoltage;
            double totalCapacitanceSeenByOutput = (2 * flybackCapacitorMinimumCapacitanceForTolerance / Constants.NStages);
            double energyWhenDesiredMaximumOutputVoltageReached = 0.5 * totalCapacitanceSeenByOutput * Math.Pow(Constants.MaximumDesiredOutputVoltage, 2);

            double potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection =
                Math.Pow((energyWhenDesiredMaximumOutputVoltageReached + maxFlybackEnergyPerCycle) / (0.5 * totalCapacitanceSeenByOutput), 0.5);
            if (potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection > maximumAchievableOutputVoltage)
            {
                double energyAtMaximumAchievableOutputVoltage = 0.5 * totalCapacitanceSeenByOutput * Math.Pow(maximumAchievableOutputVoltage, 2);
                maxOutputVoltageThreshold = Math.Pow((energyAtMaximumAchievableOutputVoltage - maxFlybackEnergyPerCycle) / (0.5 * totalCapacitanceSeenByOutput), 0.5);
                Console.WriteLine("WARNING, Had to clamp the {nameof(safeOutputVoltage)} in order to safely absorb the additional energy from one cycle");
            }
            ulong onTimeMicroSeconds = (long)(1000000d * Constants.DutyCycle / Constants.Frequency);
            ulong offTimeMicroSeconds = (long)(1000000d * (1d - Constants.DutyCycle) / Constants.Frequency);
            maxOutputVoltageThreshold = Math.Floor(maxOutputVoltageThreshold);
            HVPSConfiguration configurationStruct = new HVPSConfiguration
            {
                broadcastFrequencyHz = Constants.BroadcastFrequency,
                firstStageVoltageThresholdVolts = (float)firstStageVoltageThreshold,
                maxAverageOutputPowerWatts = (float)Constants.MaximumCompositeOutputCurrentLimitingResistorAveragePower,
                maxFlybackEnergyPerCycleJouls = (float)maxFlybackEnergyPerCycle,
                maxOutputVoltageThresholdVolts = (float)maxOutputVoltageThreshold,
                defaultOutputVoltageVolts = (float)maxOutputVoltageThreshold,
                minOutputVoltageThresholdVolts = (float)Constants.MinimumDesiredOutputVoltage,
                nVillardStages = Constants.NStages,
                pingTimeoutMilliseconds = Constants.PingTimeoutMilliseconds,
                sendPingIntervalMilliseconds = Constants.SendPingIntervalMilliseconds,
                villardCapacitorCapacitanceFarads = (float)Constants.VillardCapacitorCapacitance,
                vPsOverVadcRatio = (float)Constants.PowerSupplyVoltageFeedbackPotentialDividerRatio,
                villardCapacitorsBleedTimeConstantSeconds = 
                    (uint)Math.Ceiling(
                        (1d+(Constants.VillardCapacitorTolerancePercent / 100d))
                        *Constants.VillardCapacitorCapacitance
                        *(1d + (Constants.VillardCapacitorBleedResistorTolerancePercent / 100d)) 
                        * Constants.VillardCapacitorBleedResistance),
                primaryCurrentFromRaw = (float)(
                        (Constants.ADCRawToUnscaledVoltage /
                                Constants.PrimaryCurrentFeedbackBurdenResistorResistanceOhms
                        )
                        * Constants.PrimaryCurrentFeedbackCurrentTransformerRatioSecondaryToPrimary
                    ),
                firstStageVoltageFromRaw=
                    (float)(Constants.FirstStageVoltageFeedbackPotentialDividerRatio
                    * Constants.ADCRawToUnscaledVoltage),
                outputVoltageFromRaw = 
                    (float)(Constants.OutputVoltageFeedbackPotentialDividerRatio
                    * Constants.ADCRawToUnscaledVoltage),
                maxTemperatureMosfetDegreesC = Constants.MaxTemperatureMosfet,


                errorColour = Constants.ErrorColour.ToUInt32(),
                liveColour = Constants.LiveColour.ToUInt32(),
                idleColour = Constants.IdleColour.ToUInt32(),
                runningSystemChecksColour = Constants.RunningSystemChecksColour.ToUInt32(),
                shutDownColour = Constants.ShutDownColour.ToUInt32(),
                shuttingDownColour = Constants.ShuttingDownColour.ToUInt32(),
                unknownColour = Constants.UnknownColour.ToUInt32(),


                idleFlashDelayMs = FlashHzToMilliseconds(
                    Constants.IdleFlashHz),
                liveFlashDelayMs = FlashHzToMilliseconds(
                    Constants.LiveFlashHz),
                runningSystemChecksFlashDelayMs = FlashHzToMilliseconds(
                    Constants.RunningSystemChecksFlashHz),
                shuttingDownFlashDelayMs = FlashHzToMilliseconds(
                    Constants.ShuttingDownFlashHz),
                shutDownFlashDelayMs = FlashHzToMilliseconds(
                    Constants.ShutDownFlashHz),
                errorFlashDelayMs = FlashHzToMilliseconds(
                    Constants.ErrorFlashHz),
                unknownFlashDelayMs = FlashHzToMilliseconds(
                    Constants.UnknownFlashHz),
            };
            GenerateCPlusPlusConfigurations(reposDirectory, configurationStruct);
            GenerateFPGAConstants(reposDirectory, configurationStruct);
        }
        private static void GenerateCPlusPlusConfigurations(string reposDirectory, 
            HVPSConfiguration configurationStruct) {
            string dependenciesIncludePathPrefix = "";
            AlreadyWroteWatcher alreadyWroteWatcher = new AlreadyWroteWatcher();
            CPlusPlusConfigurationWriter.WriteConfigurationStructFile<HVPSConfiguration>(Path.Combine(
                    reposDirectory,
                    "hvps",
                    "Code",
                    "C++",
                    "HVPSController2",
                    "main",
                    "Generated",
                    "HVPSConfiguration.hpp"
            ), alreadyWroteWatcher);
            {
                CPlusPlusConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath: Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "C++",
                        "HVPSController2",
                        "main",
                        "Generated",
                        "HVPSConfig.hpp"
                ),
                    configurationStruct,
                    structHppFileRelativePath: "HVPSConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher
                );
            }
            CPlusPlusEnumWriter.Write<FPGACommand>(
                Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "C++",
                        "HVPSController2",
                        "main",
                        "Generated",
                        "Enums"
                )
            );
            CPlusPlusEnumWriter.Write<FPGAState>(
                Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "C++",
                        "HVPSController2",
                        "main",
                        "Generated",
                        "Enums"
                )
            );
        }
        private static void GenerateFPGAConstants(string reposDirectory, HVPSConfiguration configurationStruct) {
            int maxPrimaryCurrent = (int)Math.Floor(Constants.FlybackTransformerMaximumCurrent / configurationStruct.primaryCurrentFromRaw);
            int maxFirstStageVoltage = (int)Math.Floor(configurationStruct.firstStageVoltageThresholdVolts/configurationStruct.firstStageVoltageFromRaw);
            int maxOutputVoltage = (int)Math.Floor(configurationStruct.maxOutputVoltageThresholdVolts / configurationStruct.outputVoltageFromRaw);
            FPGAConstantsGenerator.ConstantsGenerator.Generate(Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "Verilog",
                        "HVPSController2",
                        "GeneratedConstants.sv"
                ),
                new Constant[] {
                    new Constant(name: "MAX_PRIMARY_CURRENT", value: maxPrimaryCurrent, Format.Decimal, nBits: 8),
                    new Constant(name: "MAX_FIRST_STAGE_VOLTAGE", value: maxFirstStageVoltage, Format.Decimal, nBits: 8),
                    new Constant(name: "MAX_OUTPUT_VOLTAGE", value: maxFirstStageVoltage, Format.Decimal, nBits: 8)
                }
                .Concat(FPGAConstantsGenerator.ConstantsFactory.FromEnum<FPGACommand>(8))
                .Concat(FPGAConstantsGenerator.ConstantsFactory.FromEnum<FPGAState>(8))
                .ToArray()
            );
        }
        private static UInt32 FlashHzToMilliseconds(double hz){
            if (hz <= 0) return 0;
            double delayMs = Math.Ceiling((double)500 / hz);
            if (delayMs <= 0) return 0;
            var delay = (UInt32)delayMs;
            return delay;
        }
    }
}
