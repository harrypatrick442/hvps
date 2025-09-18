export default class PingMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="p";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}