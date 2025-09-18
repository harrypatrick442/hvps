export default class HVPSStartMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="start";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}