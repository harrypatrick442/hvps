using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class StartMessage : TypedMessageBase
    {
        public StartMessage()
            : base()
        {
            Type = MessageTypes.Start;
        }
    }
}
