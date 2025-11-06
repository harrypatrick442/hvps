export default class Message
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="m";
    r["c"]=o.content;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.content=o["c"];
   return r;
 }
}