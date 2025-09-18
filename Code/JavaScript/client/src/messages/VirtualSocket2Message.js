export default class VirtualSocket2Message
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="vs3m";
    r["s"]=o.secret;
    r["i"]=o.endpointId;
    r["n"]=o.theirNodeId;
    r["p"]=o.payload;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.secret=o["s"];
    r.endpointId=o["i"];
    r.theirNodeId=o["n"];
    r.payload=o["p"];
   return r;
 }
}