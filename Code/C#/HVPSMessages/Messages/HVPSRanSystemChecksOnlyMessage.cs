using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSUIRanSystemChecksMessage : TypedMessageBase
    {
        public HVPSUIRanSystemChecksMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIRanSystemChecks;
        }
    }
}
