export default class GetVoltageRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="gv";
    r["v"]=o.voltage;
    return r;
 }
}