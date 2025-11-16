using Core.Messages.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class ClearLoggedErrorsMessage : TypedMessageBase
    {
        public ClearLoggedErrorsMessage()
            : base()
        {
            Type = MessageTypes.ClearLoggedErrors;
        }
    }
}
