
namespace ProgrammingHelper
{
    public class Esp32ConsoleBridge : ConsoleBridge
    {
        public Esp32ConsoleBridge(
            string workingDirectory, Action callbackExited) :
            base(workingDirectory, callbackExited)
        {
        }
        public void SendConfigureEnvironmentCommands() {
            base.Send($"set IDF_PATH=C:\\Users\\USER1\\esp\\v5.4\\esp-idf");
            base.Send($"call %IDF_PATH%\\export.bat");
        }
        public void SendBuild() {
            base.Send($"idf.py build");
        }
        public void SendFlash(string comPort) {

            base.Send($"idf.py -p {comPort} flash");
        }
        public void SendMonitor(string comPort){
            base.Send($"idf.py -p {comPort} monitor");
        }
        public void SendControlPlusBracket()
        {
            base.Send(((char)0x1D).ToString());
        }
        public void SendControlPlusC()
        {
            base.Send(((char)0x03).ToString());
        }
    }
}
