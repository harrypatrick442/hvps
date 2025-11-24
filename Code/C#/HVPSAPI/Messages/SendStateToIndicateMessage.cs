using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSAPI.DataMemberNames.Responses;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class SendStateToIndicateMessage : TypedMessageBase
    {
        public SendStateToIndicateMessage()
            : base()
        {
            Type = MessageTypes.SendStateToIndicate;
        }
    }
}
