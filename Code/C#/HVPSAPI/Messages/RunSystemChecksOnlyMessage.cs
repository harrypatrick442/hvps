using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class RunSystemChecksOnlyMessage : TypedMessageBase
    {
        public RunSystemChecksOnlyMessage()
            : base()
        {
            Type = MessageTypes.RunSystemChecksOnly;
        }
    }
}
