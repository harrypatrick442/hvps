using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSStopMessage : TypedMessageBase
    {
        public HVPSStopMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIStop;
        }
    }
}
