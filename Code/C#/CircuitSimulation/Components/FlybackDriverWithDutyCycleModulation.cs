using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Components.Bases;
using ModifiedNodalAnalysis.Nodes;

namespace VoltageMultiplier.CircuitSimulation.Components
{
    public class FlybackDriverWithDutyCycleModulation : FlybackDriverBase
    {
        private PIControllerConstants _PiControllerConstants;

        public FlybackDriverWithDutyCycleModulation(
            string name,
            NodeBase nodeGround, NodeBase nodeTransformerLowSide,
            Circuit circuit,
            double f_sw,
            double D_min,
            double D_max,
            Resistor? rSenseSecondaryCurrent,
            Capacitor? firstVillardCapacitor,
            NonGroundNode? outputNode,
            PIControllerConstants piControllerConstants
            )
            : base(name, nodeGround, nodeTransformerLowSide,
            circuit, f_sw, D_min,
            D_max,
             rSenseSecondaryCurrent,
             firstVillardCapacitor,
             outputNode)
        {
            _PiControllerConstants = piControllerConstants;
        }
        protected override double CalculateDutyCycleFromSecondaryCurrent(double[] lastCalculatedUnknowns)
        {
            double value = _PiControllerConstants.PSecondaryCurrent * CalculateSecondaryCurrent(lastCalculatedUnknowns);
            return ClampDutyCycle(value);
        }
        protected override double CalculateDutyCycleFromFirstStageVoltage(double[] lastCalculatedUnknowns)
        {
            double value = _PiControllerConstants.PFirstStageVoltage * CalculateFirstStageVoltage(lastCalculatedUnknowns);
            return ClampDutyCycle(value);
        }
        protected override double CalculateDutyCycleFromOutputVoltage(double[] lastCalculatedUnknowns)
        {
            double value = _PiControllerConstants.POutputVoltage * CalculateOutputVoltage(lastCalculatedUnknowns);
            return ClampDutyCycle(value);
        }
        private double ClampDutyCycle(double value)
        {
            return value > D_max ? D_max : value < D_min ? D_min : value;
        }
    }
}
