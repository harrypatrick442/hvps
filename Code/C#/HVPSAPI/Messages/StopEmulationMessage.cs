using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class StopEmulationMessage : TypedMessageBase
    {
        public StopEmulationMessage()
            : base()
        {
            Type = MessageTypes.StopEmulation;
        }
    }
}
