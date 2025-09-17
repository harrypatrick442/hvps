namespace VoltageMultiplier.FlybackTransformerModelling
{
    public class FEMResultFlybackSpecs
    {
        public double L_ms { get; }
        public double L_ks { get; }
        public FEMResultFlybackSpecs(double L_ms, double L_ks)
        {
            this.L_ms = L_ms;
            this.L_ks = L_ks;
        }
    }
}