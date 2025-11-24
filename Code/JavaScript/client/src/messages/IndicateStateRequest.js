export default class IndicateStateRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="isr";
    r["s"]=o.systemState;
    return r;
 }
}