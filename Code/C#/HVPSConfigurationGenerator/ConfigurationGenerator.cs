using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Checksums;
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
            double energyWhenDesiredMaximumOutputVoltageReached = 0.5 * totalCapacitanceSeenByOutput * Math.Pow(Constants.MaximumDesiredOutputVoltage, 2);

            double potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection =
                Math.Pow((energyWhenDesiredMaximumOutputVoltageReached + maxFlybackEnergyPerCycle) / (0.5 * totalCapacitanceSeenByOutput), 0.5);
            if (potentialOutputVoltageReachedDuringOutputVoltageThresholdReachedDetection > maximumAchievableOutputVoltage) {
                double energyAtMaximumAchievableOutputVoltage = 0.5 * totalCapacitanceSeenByOutput * Math.Pow(maximumAchievableOutputVoltage, 2);
                maxOutputVoltageThreshold = Math.Pow((energyAtMaximumAchievableOutputVoltage - maxFlybackEnergyPerCycle) / (0.5 * totalCapacitanceSeenByOutput), 0.5);
                Console.WriteLine("WARNING, Had to clamp the {nameof(safeOutputVoltage)} in order to safely absorb the additional energy from one cycle");
            }
            ulong onTimeMicroSeconds = (long)(1000000d * Constants.DutyCycle / Constants.Frequency);
            ulong offTimeMicroSeconds = (long)(1000000d * (1d - Constants.DutyCycle) / Constants.Frequency);
            maxOutputVoltageThreshold = Math.Floor(maxOutputVoltageThreshold);
            HVPSConfig configurationStruct = new HVPSConfig {
                onTimeMicroSeconds= onTimeMicroSeconds,
                offTimeMicroSeconds= offTimeMicroSeconds,
                maxOutputVoltageThreshold= maxOutputVoltageThreshold,
                minOutputVoltageThreshold= Constants.MinimumDesiredOutputVoltage,
                firstStageVoltageThreshold= firstStageVoltageThreshold,
                maxAverageOutputPower= Constants.MaximumCompositeOutputCurrentLimitingResistorAveragePower,
                nStages= Constants.NStages
            };
            WriteFile(
                filePath,
                configurationStruct
            );
        }
        private static void WriteFile<TConfigurationStruct>(
            string filePath,
            TConfigurationStruct configurationStruct
        ) where TConfigurationStruct: unmanaged
        {


            uint crc = Crc32.Compute(in configurationStruct);
            StringBuilder sbStruct = new StringBuilder();
            StringBuilder sbInstance = new StringBuilder();
            Atomic<bool> isFirst = new Atomic<bool>(true);
            sbStruct.AppendLine("#pragma once");
            sbStruct.AppendLine("#include \"../System/Aborter.hpp\"");
            sbStruct.AppendLine("struct Configuration {");
            sbInstance.AppendLine("inline constexpr Configuration Config1{");
            sbInstance.Append("    ");
            foreach (var field in typeof(HVPSConfig).GetFields(BindingFlags.Public | BindingFlags.Instance))
            {
                string cPlusPlusTypeName = GetCPlusPlusTypeName(field.FieldType);
                string fieldName = field.Name;
                object? value = field.GetValue(configurationStruct);
                sbStruct.AppendLine($"    {cPlusPlusTypeName} {fieldName};");
                if (isFirst.Value)
                {
                    isFirst.Value = false;
                }
                else
                {
                    sbInstance.Append(",");
                }
                sbInstance.Append($".{fieldName} = {value}");
            }
            sbStruct.AppendLine("};");
            sbInstance.AppendLine();
            sbInstance.AppendLine("};");
            StringBuilder sbAll = new StringBuilder();
            sbAll.Append(sbStruct);
            sbAll.Append(sbInstance);
            sbAll.AppendLine($"inline const uint32_t CONFIG_CRC32_EXPECTED = {crc};");
            //sbAll.AppendLine("static_assert(podConfig1 == CONFIG_CRC32_EXPECTED, \"pod did not match expected crc\"");
            sbAll.AppendLine("inline Configuration Config2 = Config1;//This one is in RAM. Config1 is in ROM.");
sbAll.Append(@"bool validateConfiguration(){
	uint32_t podConfig1 = Crc32::computePod(Config1);
	uint32_t podConfig2 = Crc32::computePod(Config2);
	if(podConfig1 != podConfig2){
		Aborter::safeAbort(""Configuration"", ""The CRC32 computed for Config2 did not match the CRC32 for Config1"");
		return false;
	}
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
		Aborter::safeAbort(""Configuration"", ""The CRC32 computed for Config1 did not match the expected value"");
		return false;
	};
    return true;
}");
            File.WriteAllText(filePath, sbAll.ToString());
        }
        private static string GetCPlusPlusTypeName(Type fieldType)
        {
            if (fieldType == typeof(byte)) return "uint8_t";
            if (fieldType == typeof(sbyte)) return "int8_t";
            if (fieldType == typeof(short)) return "int16_t";
            if (fieldType == typeof(ushort)) return "uint16_t";
            if (fieldType == typeof(int)) return "int32_t";
            if (fieldType == typeof(uint)) return "uint32_t";
            if (fieldType == typeof(long)) return "int64_t";
            if (fieldType == typeof(ulong)) return "uint64_t";

            if (fieldType == typeof(float)) return "float";
            if (fieldType == typeof(double)) return "double";
            if (fieldType == typeof(decimal)) return "double"; // no native decimal in C++, map to double

            throw new NotSupportedException($"Unsupported type: {fieldType.FullName}");
        }
    }
}
