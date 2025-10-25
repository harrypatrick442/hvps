export default class SetForceVoltageThresholdReachedFeedbackRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="sft";
    r["f"]=o.force;
    return r;
 }
}