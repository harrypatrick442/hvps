export default class ConsoleMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="cmsg";
    r["m"]=o.message;
    r["e"]=o.isError;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.message=o["m"];
    r.isError=o["e"];
   return r;
 }
}