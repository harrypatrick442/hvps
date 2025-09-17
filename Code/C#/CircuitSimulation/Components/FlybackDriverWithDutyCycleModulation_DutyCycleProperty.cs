using ModifiedNodalAnalysis.Interfaces;
using ModifiedNodalAnalysis.Results;
using Core.Enums;

namespace VoltageMultiplier.CircuitSimulation.Components
{
    public class FlybackDriverWithDutyCycleModulation_DutyCycleProperty : INonRowFundamentalProperty
    {
        public delegate double DelegateCalculateDutyCycle(double[] unknowns);
        private DelegateCalculateDutyCycle _CalculateDutyCycle;

        public string ValueName { get; }

        public UnitsType UnitsType => UnitsType.None;

        public string ElementName { get; }

        public double CurrentValue { get; private set; }
        public double CurrentTime { get; private set; }
        private FlybackDriverBase _FlybackDriver;

        public FlybackDriverWithDutyCycleModulation_DutyCycleProperty(
            FlybackDriverBase flybackDriver, string valueName, DelegateCalculateDutyCycle calculateDutyCycle)
        {
            _FlybackDriver = flybackDriver;
            ElementName = flybackDriver.ElementName;
            ValueName = valueName;
            _CalculateDutyCycle = calculateDutyCycle;
        }
        public void Initialize(double initialDutyCycle)
        {
            CurrentValue = initialDutyCycle;
        }
        public double[] GetValuesForRange(ResultRange range, ResultsHandler resultsHandler)
        {
            return resultsHandler.GetValues(range, this);
        }
        public double[] GetValuesForRange(ResultsIndexRange indexRange, ResultsHandler resultsHandler)
        {
            return resultsHandler.GetValues(indexRange, this);
        }

        public void Calculate(double[] unknowns, double time)
        {
            long nPeriodThis = (long)Math.Floor(time / _FlybackDriver.TPeriod);
            long nPeriodLastResult = (long)Math.Floor(CurrentTime / _FlybackDriver.TPeriod);
            if (nPeriodThis <= nPeriodLastResult)
            {
                return;
            }
            CurrentValue = _CalculateDutyCycle(unknowns);
            CurrentTime = time;
        }

        public void SetCurrentValue(double value, double atTime)
        {
            CurrentValue = value;
            CurrentTime = atTime;
        }

        public IEnumerable<ResultsIndexRange> GetIndexRangesForCondition(ResultsHandler resultsHandler, Func<double, bool> predicate, int padding)
        {
            double[] values = GetValuesForRange(ResultRange.All(), resultsHandler);
            return ResultsFilteringHelper.GetIndexRangesForCondition(values, predicate, padding);
        }
        public void GetAverageMaxMin(
            ResultsHandler resultsHandler,
            out double average, out double maxValue, out int maxValueIndex,
            out double minValue, out int minValueIndex)
        {
            ResultsAveragingHelper.GetAverageMaxMin(this, resultsHandler,
                out average, out maxValue, out maxValueIndex, out minValue,
                out minValueIndex);
        }
    }
}
