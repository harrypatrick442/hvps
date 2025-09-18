export default class NewVirtualSocket2
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="vs3m";
    r["s"]=o.secret;
    r["i"]=o.endpointId;
    r["n"]=o.theirNodeId;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.secret=o["s"];
    r.endpointId=o["i"];
    r.theirNodeId=o["n"];
   return r;
 }
}