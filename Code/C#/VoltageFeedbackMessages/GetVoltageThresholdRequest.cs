using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(GetVoltageThresholdRequestDataMemberNames))]
    public class GetVoltageThresholdRequest
    {
    }
}


