using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Components.Bases;
using ModifiedNodalAnalysis.Interfaces;
using ModifiedNodalAnalysis.Nodes;

namespace VoltageMultiplier.CircuitSimulation.Components
{
    public class BangBangFlybackDriver : FlybackDriverBase
    {
        private double _DesiredOutputVoltage;
        private double _MaximumSecondaryCurrent;
        private double _MaximumFirstStateVoltage;


        public BangBangFlybackDriver(
            string name,
            NodeBase nodeGround, NodeBase nodeTransformerLowSide,
            Circuit circuit,
            double f_sw,
            double D_min,
            double D_max,
            Resistor? rSenseSecondaryCurrent,
            Capacitor? firstVillardCapacitor,
            NonGroundNode? outputNode,
            double desiredOutputVoltage,
            double maximumSecondaryCurrent,
            double maximumFirstStateVoltage,
            double t_rise = 0,
            double t_fall = 0
            )
            : base(name, nodeGround, nodeTransformerLowSide,
            circuit, f_sw,
            D_min, D_max,
             rSenseSecondaryCurrent,
             firstVillardCapacitor,
             outputNode,
             t_rise: t_rise, t_fall: t_fall)
        {
            _DesiredOutputVoltage = desiredOutputVoltage;
            _MaximumSecondaryCurrent = maximumSecondaryCurrent;
            _MaximumFirstStateVoltage = maximumFirstStateVoltage;
        }
        protected override double CalculateDutyCycleFromSecondaryCurrent(double[] lastCalculatedUnknowns)
        {
            return _RSenseSecondaryCurrent == null ? D_max : CalculateSecondaryCurrent(lastCalculatedUnknowns) > _MaximumSecondaryCurrent
                ? D_min : D_max;
        }
        protected override double CalculateDutyCycleFromFirstStageVoltage(double[] lastCalculatedUnknowns)
        {
            return _FirstVillardCapacitor == null ? D_max : CalculateFirstStageVoltage(lastCalculatedUnknowns) > _MaximumFirstStateVoltage
                ? D_min : D_max;
        }
        protected override double CalculateDutyCycleFromOutputVoltage(double[] lastCalculatedUnknowns)
        {
            if (_OutputNode == null) return D_max;
            double outputVoltage = CalculateOutputVoltage(lastCalculatedUnknowns);
            double dutyCycle = outputVoltage > _DesiredOutputVoltage
                ? D_min : D_max;
            return dutyCycle;
        }
    }
}
