using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class StartEmulationMessage : TypedMessageBase
    {
        public StartEmulationMessage()
            : base()
        {
            Type = MessageTypes.StartEmulation;
        }
    }
}
