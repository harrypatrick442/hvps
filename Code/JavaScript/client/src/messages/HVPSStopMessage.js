export default class HVPSStopMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="stop";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}