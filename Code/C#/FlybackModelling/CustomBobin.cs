using CircuitAnalysis.ComponentSpecs;

namespace FlybackModelling
{
    // Example usage:
    public class CustomBobin : BobinSpecs
    {
        public CustomBobin()
        {

        }

        public override double WidthToCore =>0.002;

        public override double LengthForWindings => 0.08;

        public override string DataSheetUrl => throw new NotImplementedException();
    }
}