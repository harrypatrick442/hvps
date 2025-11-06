export default class ErrorMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="e";
    r["t"]=o.errorType;
    r["s"]=o.serializedError;
    r["r"]=o.reason;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.errorType=o["t"];
    r.serializedError=o["s"];
    r.reason=o["r"];
   return r;
 }
}