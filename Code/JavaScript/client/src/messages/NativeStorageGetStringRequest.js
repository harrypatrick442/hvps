export default class NativeStorageGetStringRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="nsgs";
    r["k"]=o.key;
    return r;
 }
}