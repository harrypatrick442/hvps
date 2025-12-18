using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using HVPSConstants;
namespace HVPSConfigurationGenerator
{
    class Program
    {

        static void Main(string[] args)
        {
            string dependenciesIncludePathPrefix = "";
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
                minOutputVoltageThresholdVolts = (float)Constants.MinimumDesiredOutputVoltage,
                nVillardStages = Constants.NStages,
                onTimeMicroSeconds = onTimeMicroSeconds,
                offTimeMicroSeconds = offTimeMicroSeconds,
                pingTimeoutMilliseconds = Constants.PingTimeoutMilliseconds,
                villardCapacitorCapacitanceFarads = (float)Constants.VillardCapacitorCapacitance,
                vPsOverVadcRatio = (float)Constants.PowerSupplyVoltageFeedbackPotentialDividerRatio,
            };
            VoltageFeedbackModuleConfiguration firstStageVoltageFeedbackModuleConfigStruct = new VoltageFeedbackModuleConfiguration
            {
                broadcastFrequencyHz = Constants.BroadcastFrequency,
                defaultThreshold = (float)firstStageVoltageThreshold,
                vHvOverVadcRatio = (float)Constants.FirstStageVoltageFeedbackPotentialDividerRatio
            };
            VoltageFeedbackModuleConfiguration outputVoltageFeedbackModuleConfigStruct = new VoltageFeedbackModuleConfiguration
            {
                broadcastFrequencyHz = Constants.BroadcastFrequency,
                defaultThreshold = (float)maxOutputVoltageThreshold,
                vHvOverVadcRatio = (float)Constants.OutputVoltageFeedbackPotentialDividerRatio
            };
            AlreadyWroteWatcher alreadyWroteWatcher = new AlreadyWroteWatcher();
#region Write_HVPSConfiguration
#region To_HVPSController
            ConfigurationWriter.WriteConfigurationStructFile<HVPSConfiguration>(Path.Combine(
                    reposDirectory,
                    "hvps",
                    "Code",
                    "C++",
                    "HVPSController",
                    "main",
                    "Generated",
                    "HVPSConfiguration.hpp"
            ), alreadyWroteWatcher);
            {
                ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath: Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "C++",
                        "HVPSController",
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
            #endregion
#region To_Peripheral1
            ConfigurationWriter.WriteConfigurationStructFile<HVPSConfiguration>(Path.Combine(
                    reposDirectory,
                    "hvps",
                    "Code",
                    "C++",
                    "Peripheral1",
                    "main",
                    "Generated",
                    "HVPSConfiguration.hpp"
            ), alreadyWroteWatcher);
            {
                ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath: Path.Combine(
                        reposDirectory,
                        "hvps",
                        "Code",
                        "C++",
                        "Peripheral1",
                        "main",
                        "Generated",
                        "HVPSConfig.hpp"
                ),
                    configurationStruct,
                    structHppFileRelativePath: "HVPSConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher,
                    instancePrefix:"HVPSConfig"
                );
            }
            #endregion
            #endregion
#region Write_VoltageFeedbackModuleConfigurationStruct
#region To_VoltageFeedbackModuleBase
            ConfigurationWriter.WriteConfigurationStructFile<VoltageFeedbackModuleConfiguration>(
                Path.Combine(reposDirectory, "hvps", "Code", "C++", "VoltageFeedbackModuleBase",
                    "Generated", "VoltageFeedbackModuleConfiguration.hpp"),
                alreadyWroteWatcher
            );
            #endregion
#region To_Peripheral1
            ConfigurationWriter.WriteConfigurationStructFile<VoltageFeedbackModuleConfiguration>(
                Path.Combine(reposDirectory, "hvps", "Code", "C++", "Peripheral1", "main",
                    "Generated", "VoltageFeedbackModuleConfiguration.hpp"),
                alreadyWroteWatcher
            );
            #endregion
#endregion
#region Write_FirstStageVoltageFeedbackModuleConfig
            #region To_FirstStageVoltageFeedbackModule
            ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath:
                        Path.Combine(reposDirectory, "hvps", "Code", "C++",
                        "FirstStageVoltageFeedbackModule", "main",
                        "Generated", "FirstStageVoltageFeedbackModuleConfig.hpp"),
                    firstStageVoltageFeedbackModuleConfigStruct,
                    structHppFileRelativePath: "Generated/VoltageFeedbackModuleConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher

                );
#endregion
#region To_Peripheral1
            ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath:
                        Path.Combine(reposDirectory, "hvps", "Code", "C++",
                        "Peripheral1", "main",
                        "Generated", "FirstStageVoltageFeedbackModuleConfig.hpp"),
                    firstStageVoltageFeedbackModuleConfigStruct,
                    structHppFileRelativePath: "Generated/VoltageFeedbackModuleConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher,
                    instancePrefix:"FirstStageVoltageFeedbackModuleConfig"
                );
#endregion
#endregion
#region Write_OutputVoltageFeedbackModuleConfig
#region To_OutputVoltageFeedbackModule
                ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath:
                        Path.Combine(reposDirectory, "hvps", "Code", "C++",
                        "OutputVoltageFeedbackModule", "main",
                        "Generated", "OutputVoltageFeedbackModuleConfig.hpp"),
                    outputVoltageFeedbackModuleConfigStruct,
                    structHppFileRelativePath: "Generated/VoltageFeedbackModuleConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher

                );
                #endregion
#region To_Peripherial1
                ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath:
                        Path.Combine(reposDirectory, "hvps", "Code", "C++",
                        "Peripheral1", "main",
                        "Generated", "OutputVoltageFeedbackModuleConfig.hpp"),
                    outputVoltageFeedbackModuleConfigStruct,
                    structHppFileRelativePath: "Generated/VoltageFeedbackModuleConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher,
                    instancePrefix:"OutputVoltageFeedbackModuleConfig"

                );
            #endregion
            #endregion
            /*
            ConfigurationWriter.WriteConfigurationStructFile<CurrentFeedbackModuleConfig>(
                Path.Combine(reposDirectory, "hvps", "Code", "C++", "CurrentFeedbackModuleBase",
                    "Generated", "CurrentFeedbackModuleConfiguration.hpp"),
                alreadyWroteWatcher
            );*/
            /*
            {
                CurrentFeedbackModuleConfig outputCurrentFeedbackModuleConfigStruct = new CurrentFeedbackModuleConfig
                {
                    senseResistanceOhms = Constants.SenseResistance,
                    outputCurrentLimitingResistanceOhms=
                        Constants.NSeriesOutputCurrentLimitingResistors*
                        Constants.IndividualOutputCurrentLimitingResistorResistance,
                    cumulativeEnergyThresholdJ = Constants.OutputResistorMaximumEnergy,
                    energyDisipatedJPerS = Constants.OutputCurrentLimitingResistorMaxPowerDisipation,
                    broadcastFrequencyHz = Constants.BroadcastFrequency
                };
                ConfigurationWriter.WriteProjectSpecificConfiguration(
                    projectSpecificConfigurationFilePath:
                        Path.Combine(reposDirectory, "hvps", "Code", "C++",
                        "OutputCurrentFeedbackModule", "main",
                        "Generated", "OutputCurrentFeedbackModuleConfig.hpp"),
                    outputCurrentFeedbackModuleConfigStruct,
                    structHppFileRelativePath: "Generated/CurrentFeedbackModuleConfiguration.hpp",
                    dependenciesIncludePathPrefix,
                    alreadyWroteWatcher

                );
            }*/
            WritePeripheral1(reposDirectory,
                dependenciesIncludePathPrefix,
                alreadyWroteWatcher);
        }
        private static void WritePeripheral1(string reposDirectory,
            string dependenciesIncludePathPrefix, 
            AlreadyWroteWatcher alreadyWroteWatcher) {

            Peripheral1Configuration peripheral1Config = new Peripheral1Configuration
            {
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
            ConfigurationWriter.WriteProjectSpecificConfiguration(
                projectSpecificConfigurationFilePath:
                    Path.Combine(reposDirectory, "hvps", "Code", "C++",
                    "Peripheral1", "main",
                    "Generated", "Peripheral1Config.hpp"),
                peripheral1Config,
                structHppFileRelativePath: "Peripheral1Configuration.hpp",
                dependenciesIncludePathPrefix,
                alreadyWroteWatcher
            );
            ConfigurationWriter.WriteConfigurationStructFile<Peripheral1Configuration>(
                Path.Combine(reposDirectory, "hvps", "Code", "C++", "Peripheral1",
                    "main",
                    "Generated", "Peripheral1Configuration.hpp"),
                alreadyWroteWatcher
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
