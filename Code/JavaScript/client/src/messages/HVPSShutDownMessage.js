export default class HVPSShutDownMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="sd";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}