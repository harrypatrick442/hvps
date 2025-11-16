export default class ClearLoggedErrorsMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="cle";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}