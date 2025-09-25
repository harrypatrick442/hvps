using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSShutDownMessage : TypedMessageBase
    {
        public HVPSShutDownMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIShutDown;
        }
    }
}
