using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class HVPSUIRanSystemChecksMessage : TypedMessageBase
    {
        public HVPSUIRanSystemChecksMessage()
            : base()
        {
            Type = MessageTypes.RanSystemChecks;
        }
    }
}
