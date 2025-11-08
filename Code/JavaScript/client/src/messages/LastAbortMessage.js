export default class LastAbortMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="cd";
    r["r"]=o.reason;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.reason=o["r"];
   return r;
 }
}