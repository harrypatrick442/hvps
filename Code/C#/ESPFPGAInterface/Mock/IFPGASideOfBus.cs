using Core;
using Initialization.Exceptions;
using Timer = System.Timers.Timer;

namespace ESPFPGAInterface.Mock
{
    public interface IFPGASideOfBus
    {
        public Wire ToOutputWire { get; }
        public Wire InValueWire { get; }
        public Wire InShiftWire { get; }
        public Wire OutShiftWire { get; }
        public Wire OutValueWire { get; }
        public Wire GoLiveWire { get; }
        public Wire EmergencyShutdownWire { get; }
    }
}
