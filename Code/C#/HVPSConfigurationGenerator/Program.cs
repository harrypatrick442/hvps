using System;
using System.IO;
using System.Linq;
using System.Reflection;
namespace HVPSConfigurationGenerator
{
    class Program
    {

        static void Main(string[] args)
        {
            string outputDirectory = Assembly.GetEntryAssembly().Location;
            while (Path.GetFileName(outputDirectory).ToLower() != "snippets")
            {
                outputDirectory = Directory.GetParent(outputDirectory)?.FullName;
            }
            string generatedDirectory = Path.Combine(outputDirectory,
                    "CircuitAnalysis",
                    "VoltageMultiplier",
                    "Code",
                    "C++",
                    "HVPSController",
                    "main",
                    "Generated");
            if (Constants.VillardCapacitorDeratedMaximumVoltage > Constants.VillardCapacitorVoltageRating / Constants.SafetyFactor) {
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
            double maximumAchievableOutputVoltage= (Constants.NStages/*Not 2* because of how is driven*/ * (firstStageVoltageThreshold - Constants.MaximumVillardDiodeForwardVoltageDrop));
            if (maximumAchievableOutputVoltage < Constants.DesiredMaximumOutputVoltage) {
                throw new Exception($"The calculated maximum achievable o0utput voltage for {Constants.NStages} is {maximumAchievableOutputVoltage} which is less than the desired maximum output voltage of {Constants.DesiredMaximumOutputVoltage}");
            }


        }
    }
}
