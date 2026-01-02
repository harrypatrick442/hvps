export default class VoltageMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="vol";
    r["v"]=o.voltage;
    r["r"]=o.rawVoltage;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.voltage=o["v"];
    r.rawVoltage=o["r"];
   return r;
 }
}