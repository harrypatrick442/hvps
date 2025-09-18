export default class NativePermissionsUpdateMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="npu";
    r["a"]=o.hasAllRequired;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.hasAllRequired=o["a"];
   return r;
 }
}