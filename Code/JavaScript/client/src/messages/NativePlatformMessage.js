export default class NativePlatformMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="npt";
    r["p"]=o.platform;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.platform=o["p"];
   return r;
 }
}