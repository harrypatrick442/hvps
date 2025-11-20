export default class GetVoltageThresholdResponse
{
 static fromJSON(o){
    const r = {};
    r.voltage=o["v"];
   return r;
 }
}