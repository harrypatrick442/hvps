export default class LastAbortMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="la";
    r["r"]=o.reason;
    r["bt"]=o.backtrace;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.reason=o["r"];
    r.backtrace=o["bt"];
   return r;
 }
}