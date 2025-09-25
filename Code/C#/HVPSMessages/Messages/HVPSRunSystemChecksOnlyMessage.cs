using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSRunSystemChecksOnlyMessage : TypedMessageBase
    {
        public HVPSRunSystemChecksOnlyMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIRunSystemChecksOnly;
        }
    }
}
