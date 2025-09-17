using CircuitAnalysis;
using CircuitAnalysis.ComponentSpecs;
using InfernoDispatcher;
using Core.MemoryManagement;
using Shutdown;
using Logging;
using VoltageMultiplier.FlybackTransformerModelling;
using ModifiedNodalAnalysis.Results;
using ModifiedNodalAnalysis.Plotting;
using VoltageMultiplier.CircuitSimulation;

namespace VoltageMultiplier
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