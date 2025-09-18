export default class GetVoltageResponse
{
 static fromJSON(o){
    const r = {};
    r.voltage=o["v"];
   return r;
 }
}