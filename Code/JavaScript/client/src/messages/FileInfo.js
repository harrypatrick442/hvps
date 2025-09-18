export default class FileInfo
{
 static toJSON(o){ 
    const r = {};
    r["n"]=o.name;
    r["s"]=o.size;
    r["t"]=o.type;
    r["l"]=o.lastModified;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.name=o["n"];
    r.size=o["s"];
    r.type=o["t"];
    r.lastModified=o["l"];
   return r;
 }
}