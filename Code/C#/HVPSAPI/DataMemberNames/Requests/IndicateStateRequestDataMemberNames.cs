using HVPSAPI.DataMemberNames.Messages;
using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Requests
{
    [MessageType(MessageTypes.IndicateStateRequest)]
    public static class IndicateStateRequestDataMemberNames
    {
        public const string SystemState = "s";
    }
}
