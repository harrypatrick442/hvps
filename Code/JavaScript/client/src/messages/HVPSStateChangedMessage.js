export default class HVPSStateChangedMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="sc";
    r["s"]=o.state;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.state=o["s"];
   return r;
 }
}