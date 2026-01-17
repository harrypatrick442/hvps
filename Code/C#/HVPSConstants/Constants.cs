using System.Runtime.InteropServices;
using Core.Graphics;

namespace HVPSConstants
{
    public class Constants
    {
        public const double SafetyFactor = 1.2;
        public const int Frequency = 16000;
        public const int BroadcastFrequency = 2;
        public const double DutyCycle = 0.5;
        public const double VillardCapacitorVoltageRating = 20000;
        public const double VillardCapacitorDeratedMaximumVoltage = 15000;
        public const double VillardCapacitorCapacitance = 1e-9;
        public const double VillardCapacitorTolerancePercent = 10;
        public const double VillardCapacitorBleedResistance = 5e9*3;
        public const double VillardCapacitorBleedResistorTolerancePercent = 10;
        public const int NStages = 6;
        public const double VillardDiodeVoltageRating = 35000;
        public const double VillardDiodeCurrentRating = 0.1;
        public const double MaximumVillardDiodeForwardVoltageDrop = 52;
        public const double FlybackTransformerMeasuredPrimaryInductances = 4.9e-5;
        public const double FlybackTransformerMaximumCurrent = 20;
        public const double MaximumDesiredOutputVoltage = 80000;
        public const double MinimumDesiredOutputVoltage = 60000;
        public const double NSeriesOutputCurrentLimitingResistors = 5;
        public const double IndividualOutputCurrentLimitingResistorResistance = 200000;
        public const double MaximumCompositeOutputCurrentLimitingResistorAveragePower = 12.5;
        public const double PowerSupplyVoltageFeedbackPotentialDividerRatio = 33;
        public const double FirstStageVoltageFeedbackPotentialDividerRatio = 5600;
        public const double OutputVoltageFeedbackPotentialDividerRatio = 3.08e4;
        //public const double OutputResistorMaximumEnergy = ;
        public const double OutputCurrentLimitingResistorMaxPowerDisipation = 15;
        public const double CurrentFeedbackSenseVoltageToCurrent = 1f/(0.001f * 57f);
        public const float MaxTemperatureMosfet = 50;
        public const float MaxTemperatureLowerSnubberDiode = 50;
        public const int PingTimeoutMilliseconds = 3000;
        public static readonly RGB IdleColour = new RGB("#24aee0");
        public const double IdleFlashHz = 2;
        public static readonly RGB LiveColour = new RGB("#ff0000");
        public const double LiveFlashHz = 2;
        public static readonly RGB RunningSystemChecksColour = new RGB("#FF1500");
        public const double RunningSystemChecksFlashHz = 1;
        public static readonly RGB ShuttingDownColour = new RGB("#8B4000");
        public const double ShuttingDownFlashHz = 1;
        public static readonly RGB ShutDownColour = new RGB("#00ff00");
        public const double ShutDownFlashHz = 0;
        public static readonly RGB ErrorColour = new RGB("#f7075f");
        public const double ErrorFlashHz = 1;
        public static readonly RGB UnknownColour = new RGB("#ffffff");
        public const double UnknownFlashHz = 1;
    }
}
