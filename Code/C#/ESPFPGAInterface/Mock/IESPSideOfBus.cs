using Core;
using Initialization.Exceptions;
using Timer = System.Timers.Timer;

namespace ESPFPGAInterface.Mock
{
    public interface IESPSideOfBus
    {
        public void SetInShift(bool onElseOff);
        public void SetInValue(bool onElseOff);
        public void SetToOutput(bool onElseOff);
        public void SetOutShift(bool onElseOff);
        public bool GetOutValue();
        public void SetGoLive(bool onElseOff);
        public bool GetEmergencyShutdown();
    }
}
