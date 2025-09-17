using System;
using System.IO;
using System.Linq;
using System.Reflection;
namespace HVPSConfigurationGenerator
{
    class Constants
    {
        public const double SafetyFactor = 1.2;
        public const double VillardCapacitorVoltageRating = 20000;
        public const double VillardCapacitorDeratedMaximumVoltage = 15000;
        public const double VillardCapacitorCapacitance = 1e-9;
        public const double VillardCapacitorTolerancePercent = 10;
        public const int NStages = 6;
        public const double VillardDiodeVoltageRating = 35000;
        public const double VillardDiodeCurrentRating = 0.1;
        public const double MaximumVillardDiodeForwardVoltageDrop = 52;
        public const double FlybackTransformerMeasuredPrimaryInductances = 4.9e-5;
        public const double FlybackTransformerMaximumCurrent = 20;
        public const double DesiredMaximumOutputVoltage = 80000;
        public const double NSeriesOutputCurrentLimitingResistors = ;

    }
}
