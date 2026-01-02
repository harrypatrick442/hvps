using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(SetForceVoltageThresholdReachedFeedbackRequestDataMemberNames))]
    public class SetForceVoltageThresholdReachedFeedbackRequest
    {
        private bool? _Force;
        [JsonPropertyName(
            SetForceVoltageThresholdReachedFeedbackRequestDataMemberNames.Force)]
        [JsonInclude]
        [DataMember(Name =
            SetForceVoltageThresholdReachedFeedbackRequestDataMemberNames.Force)]
        public bool? Force { get { return _Force; } protected set { _Force = value; } }
    }
}


