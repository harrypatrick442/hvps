using Core;
using Core.Timing;
using Initialization.Exceptions;
using Timer = System.Timers.Timer;

namespace ESPFPGAInterface.Mock
{
    public class Bus:IESPSideOfBus, IFPGASideOfBus
    {
        public Wire ToOutputWire { get; }
        public Wire InValueWire { get; }
        public Wire InShiftWire { get; }
        public Wire OutShiftWire { get; }
        public Wire OutValueWire { get; }
        public Wire GoLiveWire { get; }
        public Wire EmergencyShutdownWire { get; }
        public Bus()
        {
            ToOutputWire = new Wire();
            InValueWire = new Wire();
            InShiftWire = new Wire();
            OutShiftWire = new Wire();
            OutValueWire = new Wire();
            GoLiveWire = new Wire();
            EmergencyShutdownWire = new Wire();
        }

        public void SetInShift(bool onElseOff)
        {
            InShiftWire.Set(onElseOff);
        }

        public void SetInValue(bool onElseOff)
        {
            InValueWire.Set(onElseOff);
        }

        public void SetToOutput(bool onElseOff)
        {
            ToOutputWire.Set(onElseOff);
        }

        public void SetOutShift(bool onElseOff)
        {
            OutShiftWire.Set(onElseOff);
        }

        public bool GetOutValue()
        {
            return OutValueWire.Get();
        }

        public void SetGoLive(bool onElseOff)
        {
            GoLiveWire.Set(onElseOff);
        }

        public bool GetEmergencyShutdown()
        {
            return EmergencyShutdownWire.Get();
        }
    }
}
