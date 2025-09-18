export default class NativeProgressMessage
{
 static toJSON(o){ 
    const r = {};
    r["p"]=o.progress;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.progress=o["p"];
   return r;
 }
}