export default class SetForceVoltageThresholdReachedFeedbackResponse
{
 static fromJSON(o){
    const r = {};
    r.force=o["f"];
   return r;
 }
}