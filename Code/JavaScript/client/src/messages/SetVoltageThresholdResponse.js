export default class SetVoltageThresholdResponse
{
 static fromJSON(o){
    const r = {};
    r.voltage=o["v"];
   return r;
 }
}