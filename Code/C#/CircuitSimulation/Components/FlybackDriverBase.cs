using Core.Enums;
using Core.Maths.Tensors.Interfaces;
using ModifiedNodalAnalysis;
using ModifiedNodalAnalysis.Components.Bases;
using ModifiedNodalAnalysis.Components.FundamentalProperties;
using ModifiedNodalAnalysis.Components.StaticParameters;
using ModifiedNodalAnalysis.Interfaces;
using ModifiedNodalAnalysis.Nodes;
using System.Diagnostics;
using System.Reflection;
using DutyCycleProperty = VoltageMultiplier.CircuitSimulation.Components.FlybackDriverWithDutyCycleModulation_DutyCycleProperty;


namespace VoltageMultiplier.CircuitSimulation.Components
{
    public abstract class FlybackDriverBase : TwoNodeComponent, IInstantaneousSteadyStateComponent
    {
        public override IFundamentalProperty[] FundamentalProperties => new IFundamentalProperty[] {
            DutyCycleFromFirstStageVoltage,
            DutyCycleFromOutputVoltage,
            DutyCycleFromSecondaryCurrent,
            DutyCycle,
            Resistance
        };
        public override IInstantaneousSteadyStateComponent Instantaneous => this;
        public NodeBase NodeFrom { get; }
        public NodeBase NodeTo { get; }
        protected double D_min;
        protected double D_max;
        protected double R_max_off { get; }
        protected double R_min_on { get; }
        private double T_delay { get; }
        private double T_rise { get; }
        private double T_fall { get; }
        public double TPeriod { get; }
        protected Resistor? _RSenseSecondaryCurrent;
        protected Capacitor? _FirstVillardCapacitor;
        protected NonGroundNode? _OutputNode;
        public override ComponentStaticParameter[] StaticParameters { get; }
        private DutyCycleProperty? _DutyCycleFromSecondaryCurrent;
        public DutyCycleProperty DutyCycleFromSecondaryCurrent
        {
            get
            {
                if (_DutyCycleFromSecondaryCurrent == null)
                {
                    _DutyCycleFromSecondaryCurrent =
                        new DutyCycleProperty(
                            this,
                            "D_secondary_current",
                            CalculateDutyCycleFromSecondaryCurrent
                         );
                }
                return _DutyCycleFromSecondaryCurrent;
            }
        }
        private DutyCycleProperty? _DutyCycleFromFirstStageVoltage;
        public DutyCycleProperty DutyCycleFromFirstStageVoltage
        {
            get
            {
                if (_DutyCycleFromFirstStageVoltage == null)
                {
                    _DutyCycleFromFirstStageVoltage =
                        new DutyCycleProperty(
                            this,
                            "D_first_stage_voltage",
                            CalculateDutyCycleFromFirstStageVoltage
                         );
                }
                return _DutyCycleFromFirstStageVoltage;
            }
        }
        private DutyCycleProperty? _DutyCycleFromOutputVoltage;
        public DutyCycleProperty DutyCycleFromOutputVoltage
        {
            get
            {
                if (_DutyCycleFromOutputVoltage == null)
                {
                    _DutyCycleFromOutputVoltage =
                        new DutyCycleProperty(
                            this,
                            "D_output_voltage",
                            CalculateDutyCycleFromOutputVoltage
                         );
                }
                return _DutyCycleFromOutputVoltage;
            }
        }
        private DutyCycleProperty? _DutyCycle;
        public DutyCycleProperty DutyCycle
        {
            get
            {
                if (_DutyCycle == null)
                {
                    _DutyCycle =
                        new DutyCycleProperty(
                            this,
                            "D_output_voltage",
                            CalculateDutyCycle
                         );
                }
                return _DutyCycle;
            }
        }
        private Resistance? _Resistance;
        public Resistance Resistance
        {
            get
            {
                if (_Resistance == null)
                {
                    _Resistance =
                        new Resistance(
                            this,
                            "R_driver",
                            CalculateResistance
                         );
                }
                return _Resistance;
            }
        }
        public override IDerivedProperty[] DerivedProperties => new IDerivedProperty[] { };
        public override bool IsNonLinear => false;

        public Component ComponentFrom => this;

        public FlybackDriverBase(
            string name,
            NodeBase nodeGround, NodeBase nodeTransformerLowSide,
            Circuit circuit,
            double f_sw,
            double D_min,
            double D_max,
            Resistor? rSenseSecondaryCurrent,
            Capacitor? firstVillardCapacitor,
            NonGroundNode? outputNode,
            double R_max_off = 1e6,
            double R_min_on = 1e-2,
            double t_rise = 0,
            double t_fall = 0
            )
            : base(name,
                  nodeGround, nodeTransformerLowSide,
                  circuit)
        {
            if (R_min_on <= 0) throw new ArgumentException($"{nameof(R_min_on)} must be greater than 0");
            if (R_min_on >= R_max_off) throw new ArgumentException($"{nameof(R_max_off)} must be greater than {nameof(R_min_on)}");
            if (D_min < 0) throw new ArgumentException($"{nameof(D_min)} must be greater than or equal to 0");
            if (D_min > 1) throw new ArgumentException($"{nameof(D_min)} must be less than or equal to 1");
            if (D_max < 0) throw new ArgumentException($"{nameof(D_max)} must be greater than or equal to 0");
            if (D_max > 1) throw new ArgumentException($"{nameof(D_max)} must be less than or equal to 1");
            if (D_max < D_min) throw new ArgumentException($"{nameof(D_max)} must be greater than or equal to {nameof(D_min)}");

            if (D_max > 1) throw new ArgumentException($"{nameof(D_max)} must be less than or equal to 1");
            double tPeriod = 1d / f_sw;
            if (t_rise > tPeriod) throw new ArgumentException($"{nameof(t_rise)} must be less or equal to the period given by {nameof(f_sw)}");
            if (t_fall > tPeriod) throw new ArgumentException($"{nameof(t_fall)} must be less or equal to the period given by {nameof(f_sw)}");
            if (t_rise + t_fall > tPeriod) throw new ArgumentException($"{nameof(t_rise)}+{nameof(t_fall)} must be less or equal to the period given by {nameof(f_sw)}");
            this.D_min = D_min;
            this.D_max = D_max;
            this.R_max_off = R_max_off;
            this.R_min_on = R_min_on;
            TPeriod = tPeriod;
            T_rise = t_rise;
            T_fall = t_fall;
            NodeFrom = nodeGround;
            NodeTo = nodeTransformerLowSide;
            _RSenseSecondaryCurrent = rSenseSecondaryCurrent;
            _FirstVillardCapacitor = firstVillardCapacitor;
            _OutputNode = outputNode;
            StaticParameters = new ComponentStaticParameter[] {
                new ComponentStaticParameter(f_sw, nameof(f_sw), UnitsType.Frequency),
                new ComponentStaticParameter(D_min, nameof(D_min), UnitsType.None),
                new ComponentStaticParameter(D_max, nameof(D_max), UnitsType.None),
                new ComponentStaticParameter(R_max_off, nameof(R_max_off), UnitsType.Resistance),
                new ComponentStaticParameter(R_min_on, nameof(R_min_on), UnitsType.Resistance)
            };
            DutyCycleFromFirstStageVoltage.Initialize(initialDutyCycle: this.D_max);
            DutyCycleFromSecondaryCurrent.Initialize(initialDutyCycle: this.D_max);
            DutyCycleFromOutputVoltage.Initialize(initialDutyCycle: this.D_max);
            DutyCycle.Initialize(initialDutyCycle: this.D_max);
        }
        protected double CalculateSecondaryCurrent(double[] lastCalculatedUnknowns)
        {

            double Vr_s_sense = Math.Abs(_RSenseSecondaryCurrent!.Node2.GetVoltage(lastCalculatedUnknowns)
                    - _RSenseSecondaryCurrent.Node1.GetVoltage(lastCalculatedUnknowns));
            return Vr_s_sense * _RSenseSecondaryCurrent.Ohms;
        }
        protected double CalculateFirstStageVoltage(double[] lastCalculatedUnknowns)
        {
            double V_c_abs = Math.Abs(_FirstVillardCapacitor!.NodeTo.GetVoltage(lastCalculatedUnknowns)
                    - _FirstVillardCapacitor.NodeFrom.GetVoltage(lastCalculatedUnknowns));
            return V_c_abs;
        }
        protected double CalculateOutputVoltage(double[] lastCalculatedUnknowns)
        {

            double V_out_abs = Math.Abs(_OutputNode!.GetVoltage(lastCalculatedUnknowns));
            return V_out_abs;
        }

        private double CalculateDutyCycle(double[] lastCalculatedUnknowns)
        {
            double fromSecondaryCurrent = CalculateDutyCycleFromSecondaryCurrent(lastCalculatedUnknowns);
            double fromFirstStageVoltage = CalculateDutyCycleFromFirstStageVoltage(lastCalculatedUnknowns);
            double fromOutputVoltage = CalculateDutyCycleFromOutputVoltage(lastCalculatedUnknowns);
            double[] values = new double[] { fromSecondaryCurrent, fromFirstStageVoltage, fromOutputVoltage };
            double minCalculatedDutyCycle = values.Min();
            if (minCalculatedDutyCycle > D_max)
            {
                return D_max;
            }
            if (minCalculatedDutyCycle < D_min)
            {
                return D_min;
            }
            return minCalculatedDutyCycle;
        }

        public override double? GetMaxTimestep(double time, double smallestTimestep, double largestTimestep)
        {
            double cycleTime = time % TPeriod;
            double m = TPeriod;
            if (T_rise > 0)
            {
                m = Math.Min(m, T_rise);
            }
            if (T_fall > 0)
            {
                m = Math.Min(m, T_fall);
            }
            return m / 100d;
            double totalOnTime = DutyCycle.CurrentValue * TPeriod;
            double effectiveTrise = Math.Min(T_rise, totalOnTime / 2);
            double effectiveTfall = Math.Min(T_fall, totalOnTime / 2);
            double effectiveSteadyTime = Math.Max(0, totalOnTime - (effectiveTrise + effectiveTfall));
            if (cycleTime < effectiveTrise)//During rise
            {
                if (smallestTimestep > effectiveTrise) 
                    throw new ArgumentException($"{nameof(smallestTimestep)} ({smallestTimestep}) is greater than {nameof(effectiveTrise)} ({effectiveTrise})");
                return Math.Max(
                    Math.Min(effectiveTrise / 100d, effectiveTrise - cycleTime),
                    smallestTimestep
                );
            }
            else if (cycleTime < effectiveTrise + effectiveSteadyTime)//During on up till fall begins
            {
                double timeTillNextFallBegins = effectiveTrise + effectiveSteadyTime - cycleTime;
                if (timeTillNextFallBegins > largestTimestep) {
                    return timeTillNextFallBegins;
                }
                return Math.Max(smallestTimestep, timeTillNextFallBegins);
            }
            else if (cycleTime < effectiveTrise + effectiveSteadyTime + effectiveTfall)//During fall
            {
                if (smallestTimestep > effectiveTfall)
                    throw new ArgumentException($"{nameof(smallestTimestep)} ({smallestTimestep}) is greater than {nameof(effectiveTfall)} ({effectiveTfall})");
                return Math.Max(
                    Math.Min(effectiveTfall /100d, effectiveTrise + effectiveSteadyTime + effectiveTfall - cycleTime), 
                    smallestTimestep
                );
            }
            else//During off up till rise begins
            {
                double timeTillNextRiseBegins = TPeriod - cycleTime;
                if (timeTillNextRiseBegins > largestTimestep) {
                    return timeTillNextRiseBegins;
                }
                return Math.Max(smallestTimestep, timeTillNextRiseBegins);
            }
        }
        private double CalculateResistance(double[] unknowns, double time, double timestep)
        {
            if (time < T_delay)
            {
                return R_max_off;
            }

            double totalOnTime = DutyCycle.CurrentValue * TPeriod;
            double effectiveTrise = Math.Min(T_rise, totalOnTime / 2);
            double effectiveTfall = Math.Min(T_fall, totalOnTime / 2);
            double effectiveSteadyTime = Math.Max(0, totalOnTime - (effectiveTrise + effectiveTfall));

            time -= T_delay;
            double cycleTime = time % TPeriod;

            if (cycleTime < effectiveTrise)
            {
                return R_max_off + (R_min_on - R_max_off) * (cycleTime / effectiveTrise);
            }
            else if (cycleTime < effectiveTrise + effectiveSteadyTime)
            {
                return R_min_on;
            }
            else if (cycleTime < effectiveTrise + effectiveSteadyTime + effectiveTfall)
            {
                double a = cycleTime - effectiveTrise - effectiveSteadyTime;
                double proportionOff = (a / effectiveTfall);
                double r =  R_min_on + ((R_max_off - R_min_on) * proportionOff);
                return r;
            }
            else
            {
                return R_max_off;
            }

        }
        protected abstract double CalculateDutyCycleFromSecondaryCurrent(double[] lastCalculatedUnknowns);
        protected abstract double CalculateDutyCycleFromFirstStageVoltage(double[] lastCalculatedUnknowns);
        protected abstract double CalculateDutyCycleFromOutputVoltage(double[] lastCalculatedUnknowns);

        public override void StampStandard(
            IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly admittanceMatrix,
            IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly? jacobian,
            IAbstractEncapsulatedDoubleVectorWriteOnly knowns, double time,
            double? timestep, double[]? lastCalculatedUnknowns,
            double[]? unknownsAtStartOfSolvingNonLinear)
        {
            CheckResistanceValueAcceptable();
            double g = 1d / Resistance.CurrentValue;
            if (NodeFrom.IsNotGround)
            {
                admittanceMatrix.Increment(NodeFrom.Index, NodeFrom.Index, g);
                if (NodeTo.IsNotGround)
                {
                    admittanceMatrix.Decrement(NodeFrom.Index, NodeTo.Index, g);
                    admittanceMatrix.Increment(NodeTo.Index, NodeTo.Index, g);
                    admittanceMatrix.Decrement(NodeTo.Index, NodeFrom.Index, g);
                }
            }
            else
            {
                admittanceMatrix.Increment(NodeTo.Index, NodeTo.Index, g);
            }
            if (jacobian != null)
            {
                StampJacobianStandard(jacobian);
            }
        }

        private void StampJacobianStandard(
            IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly jacobian)
        {
            double g = 1d / Resistance.CurrentValue;
            if (NodeFrom.IsNotGround)
            {
                jacobian.Increment(NodeFrom.Index, NodeFrom.Index, g);
                if (NodeTo.IsNotGround)
                {
                    jacobian.Decrement(NodeFrom.Index, NodeTo.Index, g);
                    jacobian.Increment(NodeTo.Index, NodeTo.Index, g);
                    jacobian.Decrement(NodeTo.Index, NodeFrom.Index, g);
                }
            }
            else
            {
                jacobian.Increment(NodeTo.Index, NodeTo.Index, g);
            }
        }
        public override void StampSmallSignal(
    double[][] smallSignalAdmittanceMatrix,
    double[] smallSignalKnowns,
    double frequency,
    double[] dcOperatingPoint,
    double timeSmallSignalAnalysisHappeningAt)
        {
            CheckResistanceValueAcceptable();
            double conductance = 1.0 / Resistance.CurrentValue;

            // Stamp the conductance into the small-signal admittance matrix
            if (NodeFrom.IsNotGround)
            {
                smallSignalAdmittanceMatrix[NodeFrom.Index][NodeFrom.Index] += conductance;
                if (NodeTo.IsNotGround)
                {
                    smallSignalAdmittanceMatrix[NodeFrom.Index][NodeTo.Index] -= conductance;
                    smallSignalAdmittanceMatrix[NodeTo.Index][NodeTo.Index] += conductance;
                    smallSignalAdmittanceMatrix[NodeTo.Index][NodeFrom.Index] -= conductance;
                }
            }
            else if (NodeTo.IsNotGround)
            {
                smallSignalAdmittanceMatrix[NodeTo.Index][NodeTo.Index] += conductance;
            }
        }
        private void CheckResistanceValueAcceptable()
        {
            if (Resistance.CurrentValue <= 0)
            {
                throw new Exception("The resistance must be greater than zero at all times");
            }
        }
        public void StampInstantaneousSteadyState(
    IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly admittanceMatrix,
    IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly? jacobian,
    IAbstractEncapsulatedDoubleVectorWriteOnly knowns,
    double time, double[] instantaneousUnknowns,
    double[] standardUnknowns)
        {
            // Calculate conductance (inverse of resistance) for instantaneous steady state
            double g = 1d / Resistance.CurrentValue;

            // Stamp the admittance matrix with the calculated conductance
            if (NodeFrom.IsNotGround)
            {
                admittanceMatrix.Increment(NodeFrom.Index, NodeFrom.Index, g);

                if (NodeTo.IsNotGround)
                {
                    admittanceMatrix.Decrement(NodeFrom.Index, NodeTo.Index, g);
                    admittanceMatrix.Increment(NodeTo.Index, NodeTo.Index, g);
                    admittanceMatrix.Decrement(NodeTo.Index, NodeFrom.Index, g);
                }
            }
            else if (NodeTo.IsNotGround)
            {
                admittanceMatrix.Increment(NodeTo.Index, NodeTo.Index, g);
            }

            // Known vector remains unchanged as this component acts as a resistor
            if (jacobian != null)
            {
                StampJacobianInstantaneousSteadyState(jacobian);
            }
        }

        private void StampJacobianInstantaneousSteadyState(
    IAbstractEncapsulatedJaggedDoubleMatrixWriteOnly jacobian)
        {
            // Calculate conductance (inverse of resistance) for instantaneous steady state
            double g = 1d / Resistance.CurrentValue;

            // Stamp the Jacobian matrix with the calculated conductance
            if (NodeFrom.IsNotGround)
            {
                jacobian.Increment(NodeFrom.Index, NodeFrom.Index, g);

                if (NodeTo.IsNotGround)
                {
                    jacobian.Decrement(NodeFrom.Index, NodeTo.Index, g);
                    jacobian.Increment(NodeTo.Index, NodeTo.Index, g);
                    jacobian.Decrement(NodeTo.Index, NodeFrom.Index, g);
                }
            }
            else if (NodeTo.IsNotGround)
            {
                jacobian.Increment(NodeTo.Index, NodeTo.Index, g);
            }
        }

        public double GetStandardUnknownFromInstantaneousUnknowns(double[] instantaneousUnknowns)
        {
            throw new UnreachableException();
        }
    }

}
