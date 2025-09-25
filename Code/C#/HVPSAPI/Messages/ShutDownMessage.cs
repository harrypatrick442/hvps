using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class ShutDownMessage : TypedMessageBase
    {
        public ShutDownMessage()
            : base()
        {
            Type = MessageTypes.ShutDown;
        }
    }
}
