export default class ShutDownMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="sd";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}