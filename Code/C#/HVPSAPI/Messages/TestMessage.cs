using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class TestMessage : TypedMessageBase
    {
        public TestMessage()
            : base()
        {
            Type = MessageTypes.Test;
        }
    }
}
