export default class SendStateToIndicateMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ssti";
    return r;
 }
 static fromJSON(o){
    const r = {};
   return r;
 }
}