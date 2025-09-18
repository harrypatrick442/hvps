export default class NativeOpenDirectoryRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="nod";
    r["p"]=o.directoryPath;
    return r;
 }
}