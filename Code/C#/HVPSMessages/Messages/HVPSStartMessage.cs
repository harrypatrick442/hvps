using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSStartMessage : TypedMessageBase
    {
        public HVPSStartMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIStart;
        }
    }
}
