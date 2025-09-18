export default class VoltageMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="vol";
    r["v"]=o.voltage;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.voltage=o["v"];
   return r;
 }
}