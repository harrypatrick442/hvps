using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class RanSystemChecksMessage : TypedMessageBase
    {
        public RanSystemChecksMessage()
            : base()
        {
            Type = MessageTypes.RanSystemChecks;
        }
    }
}
