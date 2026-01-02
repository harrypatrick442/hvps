using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(SetForceVoltageThresholdReachedFeedbackResponseDataMemberNames))]
    public class SetForceVoltageThresholdReachedFeedbackResponse
    {
        private bool? _Force;
        [JsonPropertyName(
            SetForceVoltageThresholdReachedFeedbackResponseDataMemberNames.Force)]
        [JsonInclude]
        [DataMember(Name =
            SetForceVoltageThresholdReachedFeedbackResponseDataMemberNames.Force)]
        public bool? Force { get { return _Force; } protected set { _Force = value; } }
    }
}


