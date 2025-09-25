using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.Error)]
    public static class ErrorDataMemberNames
    {
        public const string Message = "m";
    }
}
