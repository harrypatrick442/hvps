export default class NativeReadyMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="nr";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}