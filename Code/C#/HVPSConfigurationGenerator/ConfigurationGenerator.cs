using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Core.Atomics;
namespace HVPSConfigurationGenerator
{
    internal class ConfigurationGenerator
    {
        public static void Generate()
        {
            string reposDirectory = Assembly.GetEntryAssembly()!.Location;
            while (Path.GetFileName(reposDirectory).ToLower() != "repos")
            {
                reposDirectory = Directory.GetParent(reposDirectory)!.FullName;
            }
            string hvpsDirectory = Path.Combine(reposDirectory, "hvps");
            string generatedDirectory = Path.Combine(hvpsDirectory,
                    "Code",
                    "C++",
                    "HVPSController",
                    "main",
                    "Generated");
            string filePath = Path.Combine(generatedDirectory, "HVPSConfig.hpp");
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
            double energyWhenDesiredMaximumOutputVoltageReached = 0.5 * totalCapacitanceSeenByOutput*Math.Pow(Constants.MaximumDesiredOutputVoltage, 2);

            double potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection = 
                Math.Pow((energyWhenDesiredMaximumOutputVoltageReached + maxFlybackEnergyPerCycle)/(0.5*totalCapacitanceSeenByOutput), 0.5);
            if (potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection > maximumAchievableOutputVoltage) {
                double energyAtMaximumAchievableOutputVoltage = 0.5 * totalCapacitanceSeenByOutput * Math.Pow(maximumAchievableOutputVoltage, 2);
                maxOutputVoltageThreshold = Math.Pow((energyAtMaximumAchievableOutputVoltage-maxFlybackEnergyPerCycle)/(0.5*totalCapacitanceSeenByOutput), 0.5);
                Console.WriteLine("WARNING, Had to clamp the {nameof(safeOutputVoltage)} in order to safely absorb the additional energy from one cycle");
            }
            long onTimeMicroSeconds = (long)(1000000d * Constants.DutyCycle / Constants.Frequency);
            long offTimeMicroSeconds = (long)(1000000d * (1d-Constants.DutyCycle) / Constants.Frequency);
            maxOutputVoltageThreshold = Math.Floor(maxOutputVoltageThreshold);
            WriteFile(
                filePath,
                onTimeMicroSeconds: onTimeMicroSeconds,
                offTimeMicroSeconds: offTimeMicroSeconds,
                maxOutputVoltageThreshold: maxOutputVoltageThreshold,
                minOutputVoltageThreshold: Constants.MinimumDesiredOutputVoltage,
                firstStageVoltageThreshold: firstStageVoltageThreshold,
                maxAverageOutputPower:Constants.MaximumCompositeOutputCurrentLimitingResistorAveragePower,
                nStages:Constants.NStages
            );
        }
        private static void WriteFile(
            string filePath,
            long onTimeMicroSeconds,
            long offTimeMicroSeconds,
            double maxOutputVoltageThreshold,
            double minOutputVoltageThreshold,
            double firstStageVoltageThreshold,
            double maxAverageOutputPower,
            short nStages
        ) {
            StringBuilder sbStruct = new StringBuilder();
            StringBuilder sbInstance = new StringBuilder();
            Atomic<bool> isFirst = new Atomic<bool>(true);
            var appendUInt64 = Create_AppendUInt64(sbStruct, sbInstance, isFirst);
            var appendDouble = Create_AppendDouble(sbStruct, sbInstance, isFirst);
            var appendUInt8 = Create_AppendUInt8(sbStruct, sbInstance, isFirst);
            sbStruct.AppendLine("#pragma once");
            sbStruct.AppendLine("struct HVPSConfig {");
            sbInstance.AppendLine("inline constexpr HVPSConfig Config1{");
            sbInstance.Append("    ");
            appendUInt64("onTimeMicroSeconds", onTimeMicroSeconds);
            appendUInt64("offTimeMicroSeconds", offTimeMicroSeconds);
            appendDouble("maxOutputVoltageThreshold", maxOutputVoltageThreshold);
            appendDouble("minOutputVoltageThreshold", minOutputVoltageThreshold);
            appendDouble("firstStageVoltageThreshold", firstStageVoltageThreshold);
            appendDouble("maxAverageOutputPower", maxAverageOutputPower);
            appendUInt8("nStages", nStages);
            sbStruct.AppendLine("};");
            sbInstance.AppendLine();
            sbInstance.AppendLine("};");
            StringBuilder sbAll = new StringBuilder();
            sbAll.Append(sbStruct);
            sbAll.Append(sbInstance);
            File.WriteAllText(filePath, sbAll.ToString());
        }
        private static Action<string, long> Create_AppendUInt64(StringBuilder sbStruct, StringBuilder sbInstance, Atomic<bool> isFirst)
        {
            return Create_Append<long>(sbStruct, sbInstance, "uint64_t", isFirst);
        }
        private static Action<string, double> Create_AppendDouble(StringBuilder sbStruct, StringBuilder sbInstance, Atomic<bool> isFirst)
        {
            return Create_Append<double>(sbStruct, sbInstance, "double", isFirst);
        }
        private static Action<string, short> Create_AppendUInt8(StringBuilder sbStruct, StringBuilder sbInstance, Atomic<bool> isFirst)
        {
            return Create_Append<short>(sbStruct, sbInstance, "uint8_t", isFirst);
        }
        private static Action<string, TValue> Create_Append<TValue>(StringBuilder sbStruct, StringBuilder sbInstance, string type, Atomic<bool> isFirst) {
            return (name, value) => {
                sbStruct.AppendLine($"    {type} {name};");
                if (isFirst.Value)
                {
                    isFirst.Value = false;
                }
                else
                {
                    sbInstance.Append(",");
                }
                sbInstance.Append($"{name} = {value}");
            };
        }
    }
}
