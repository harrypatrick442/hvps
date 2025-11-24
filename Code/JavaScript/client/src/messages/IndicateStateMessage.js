export default class IndicateStateMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="is";
    r["s"]=o.systemState;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.systemState=o["s"];
   return r;
 }
}