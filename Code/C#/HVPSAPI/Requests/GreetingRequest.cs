using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Requests
{
    [DataContract]
    public class GreetingRequest : TypedMessageBase
    {
        public GreetingRequest()
            : base()
        {
            Type = MessageTypes.GreetingRequest;
        }
    }
}
