export default class SetVoltageThresholdRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="svf";
    r["v"]=o.voltage;
    return r;
 }
}