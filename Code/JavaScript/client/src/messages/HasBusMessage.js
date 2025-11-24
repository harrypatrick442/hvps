export default class HasBusMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="hb";
    r["tgt"]=o.target;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.target=o["tgt"];
   return r;
 }
}