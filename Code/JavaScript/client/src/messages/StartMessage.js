export default class StartMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="start";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}