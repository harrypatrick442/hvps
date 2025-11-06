using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.Error)]
    public static class ErrorMessageDataMemberNames
    {
        public const string ErrorType = "t";
        public const string SerializedError = "s";
        public const string Reason = "r";
    }
}
