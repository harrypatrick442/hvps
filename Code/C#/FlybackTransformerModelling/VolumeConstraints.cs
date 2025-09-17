namespace VoltageMultiplier.FlybackTransformerModelling
{
    public class VolumeConstraints
    {
        public double WindingElement { get; }
        public double CoreElement { get; }
        public double FreeSpaceElement { get; }
        public double AirGapElement { get; }
        public VolumeConstraints(
            double windingElement, double coreElement, double freeSpaceElement, double airGapElement)
        {
            WindingElement= windingElement;
            CoreElement= coreElement;
            FreeSpaceElement= freeSpaceElement;
            AirGapElement= airGapElement;
        }
    }
}