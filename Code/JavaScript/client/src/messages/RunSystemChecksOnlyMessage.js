export default class RunSystemChecksOnlyMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="rsco";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}