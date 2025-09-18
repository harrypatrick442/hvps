export default class NativeDisposeClientToClientChannelMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ndctcc";
    r["v"]=o.virtualSocketId;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.virtualSocketId=o["v"];
   return r;
 }
}