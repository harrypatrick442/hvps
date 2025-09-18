export default class NativeGotNewTokenMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ngnt";
    r["t"]=o.token;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.token=o["t"];
   return r;
 }
}