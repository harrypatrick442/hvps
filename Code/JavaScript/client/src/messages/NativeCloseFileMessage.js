export default class NativeCloseFileMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ncf";
    r["i"]=o.identifier;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.identifier=o["i"];
   return r;
 }
}