export default class IndicateStateResponse
{
 static fromJSON(o){
    const r = {};
    r.success=o["ss"];
   return r;
 }
}