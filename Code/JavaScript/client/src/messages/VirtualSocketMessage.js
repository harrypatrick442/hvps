export default class VirtualSocketMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="v";
    r["i"]=o.id;
    r["u"]=o.internalType;
    r["p"]=o.payload;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.id=o["i"];
    r.internalType=o["u"];
    r.payload=o["p"];
   return r;
 }
}