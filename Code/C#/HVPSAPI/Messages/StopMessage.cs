using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class StopMessage : TypedMessageBase
    {
        public StopMessage()
            : base()
        {
            Type = MessageTypes.Stop;
        }
    }
}
