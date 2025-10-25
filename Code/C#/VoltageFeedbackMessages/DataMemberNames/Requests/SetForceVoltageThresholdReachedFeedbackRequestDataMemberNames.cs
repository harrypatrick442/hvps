using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.DataMemberNames.Requests
{
    [MessageType(MessageTypes.SetForceThresholdReachedFeedback)]
    public static class SetForceVoltageThresholdReachedFeedbackRequestDataMemberNames
    {
        public const string Force = "f";
    }
}


    